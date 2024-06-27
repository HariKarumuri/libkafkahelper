#include <iostream>
#include <cstring>
#include "kafkahelper.h"
#include "kafkamanager.h"
#include <signal.h>


//need to setup in parent class
//char errstr[512]; 
//rd_kafka_t *prod_rk;
//rd_kafka_conf_t *prod_conf;

//destructor to free space after termination
KafkaServiceManager::KafkaServiceManager() : prod_rk(NULL) , prod_conf(NULL) , cons_rk(NULL) ,cons_conf(NULL) , topics(NULL),kRunning(false) {
  
}

KafkaServiceManager::~KafkaServiceManager(){
    //producer instance
    if(prod_conf){
        // wait for max 10 seconds 
        rd_kafka_flush(prod_rk, 10 * 1000 );
        // Destroy the producer instance
        rd_kafka_destroy(prod_rk);
    }
    if(cons_conf){
    rd_kafka_consumer_close(cons_rk);
    rd_kafka_topic_partition_list_destroy(topics);
    rd_kafka_destroy(cons_rk);
    }
}

//making sure to create one instance of this manager and  can be used by all 
KafkaServiceManager& KafkaServiceManager::getInstance(){
    static KafkaServiceManager obj;
    //created a obj instance and return it , ensures only one is created 
    return obj;
}




void KafkaServiceManager::producer_init(std::string server_name,std::string broker_hostAndPort ){
    //if generated alredy retun it no need to re initialize it .
    if(prod_conf) return;
    
    char errstr[512];


    if(!prod_conf){
         prod_conf = rd_kafka_conf_new();  
    }

    if (rd_kafka_conf_set(prod_conf, server_name.c_str() , broker_hostAndPort.c_str(), errstr,
                          sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%s\n", errstr);
        return ;
    }

    // before setting prod_rk we call prod_rk to set the delivery report callback
   // rd_kafka_conf_set_dr_msg_cb(prod_conf, rd_kafka_delivery_report_callback);

    // Create producer instance
    prod_rk = rd_kafka_new(RD_KAFKA_PRODUCER, prod_conf, errstr, sizeof(errstr));
    if (!prod_rk) {
        fprintf(stderr, "%% Failed to create new producer: %s\n", errstr);
        return ;
    }

    // delivery report callback
    //rd_kafka_conf_set_dr_msg_cb(prod_conf, rd_kafka_delivery_report_callback);

}

void KafkaServiceManager::produce(std::string topic , std::string message, int len){

    //check whether initiated
    if(!prod_conf) return ;

    rd_kafka_resp_err_t err;

    err = rd_kafka_producev(prod_rk,RD_KAFKA_V_TOPIC(topic.c_str()),RD_KAFKA_V_VALUE((void *)message.c_str(), len),RD_KAFKA_V_END);

     if (err) {
        fprintf(stderr, "%% Failed to produce to topic %s: %s\n",
                topic, rd_kafka_err2str(err));
              } 
}

/* -----------------------------------subcriber part below ----------------------------- */

 
    
void KafkaServiceManager::consumer_init(std::string server_name,std::string broker_hostAndPort, std::string group_name , std::string group_id){

    if(cons_conf) return;

    
    char errstr[512];

    // Signal handler for clean shutdown
    signal(SIGINT, KafkaServiceManager::signal_handler);

    // Create Kafka client configuration place-holder
    cons_conf = rd_kafka_conf_new();

    // Set bootstrap broker(s)
    if (rd_kafka_conf_set(cons_conf, server_name.c_str(), broker_hostAndPort.c_str(), errstr,sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%s\n", errstr);
        return ;
    }

    // Set the consumer group id
    if (rd_kafka_conf_set(cons_conf, group_name.c_str() , group_id.c_str(), errstr,sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%s\n", errstr);
        return ;
    }

    // Create consumer instance
    cons_rk = rd_kafka_new(RD_KAFKA_CONSUMER, cons_conf, errstr, sizeof(errstr));
    if (!cons_rk) {
        fprintf(stderr, "%% Failed to create new consumer: %s\n", errstr);
        return ;
    }

}


//req is reader in dds 

//re rendered one  below

void KafkaServiceManager::consume_messages() {
    while (kRunning) {
        rd_kafka_message_t *message = rd_kafka_consumer_poll(cons_rk, 1000);
        //std::cout<<"message poll triggered above"<<std::endl;
        if (message) {
            // Call the callback function for the received message
            auto& callbackPair = kTopicCbMap[rd_kafka_topic_name(message->rkt)];
            callbackPair.first(message);  // Call the callback function

            // You can access req if needed, e.g., callbackPair.second

            rd_kafka_message_destroy(message);
        }
    }
}

// Function to stop the consumer thread
void KafkaServiceManager::stop_consumer_thread() {
    kRunning = false;
    //std::cout<<"stop_consumer_thread triggered "<<std::endl;
    if (consumerThread.joinable()) {
         //std::cout<<"stop_consumer_thread triggered and joined main thread"<<std::endl;
        consumerThread.join();
    }
     //std::cout<<"stop_consumer_thread triggered ended"<<std::endl;
}

void KafkaServiceManager::set_consumer_callback(void* req, std::string topic, int partition_size, callback_ cb) {
    // Check if consumer thread is already running
    if (kRunning) {
        KafkaServiceManager::stop_consumer_thread();  // Stop existing consumer thread
    }

    // Initialize topics list
    topics = rd_kafka_topic_partition_list_new(partition_size);
    rd_kafka_topic_partition_list_add(topics, topic.c_str(), RD_KAFKA_PARTITION_UA);

    // Subscribe to topic
    rd_kafka_resp_err_t err = rd_kafka_subscribe(cons_rk, topics);
    if (err) {
        fprintf(stderr, "%% Failed to subscribe to %s: %s\n", topic.c_str(), rd_kafka_err2str(err));
        return;
    }

    // Store callback function and request as a pair
    kTopicCbMap[topic] = {cb, req};

    // Start consuming messages in a separate thread
    kRunning = true;
    consumerThread = std::thread(&KafkaServiceManager::consume_messages,this);
}



void KafkaServiceManager::signal_handler(int sig) {
    switch (sig) {
        case SIGINT:
            fprintf(stderr, "\n%% Caught interrupt signal (Ctrl+C). Terminating consumer.\n");
            KafkaServiceManager::getInstance().stop_consumer_thread();
            exit(sig);
            break;
    }
}

