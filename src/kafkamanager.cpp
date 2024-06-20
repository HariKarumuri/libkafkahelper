#include <iostream>
#include <cstring>
#include "kafkahelper.h"
#include "kafkamanager.h"


//need to setup in parent class
//char errstr[512]; 
//rd_kafka_t *rk;
//rd_kafka_conf_t *prod_conf;

//destructor to free space after termination
KafkaServiceManager::~KafkaServiceManager(){
    //producer instance
    if(prod_conf){
        // wait for max 10 seconds 
        rd_kafka_flush(rk, 10 * 1000 );
        // Destroy the producer instance
        rd_kafka_destroy(rk);
    }
}

//making sure to create one instance of this manager and  can be used by all 
KafkaServiceManager& KafkaServiceManager::getInstance(){
    static KafkaServiceManager obj;
    //created a obj instance and return it , ensures only one is created 
    return obj;
}


//calling custom callback that to only instance or should that be each ?
// static void  rd_kafka_delivery_report_callback(rd_kafka_t *rk,const rd_kafka_message_t *rkmessage, void *opaque){

//     if (rkmessage->err) {
//         fprintf(stderr, "%% Message delivery failed: %s\n", d_kafka_err2str(rkmessage->err));
//     } else {
//         fprintf(stdout, "%% Message delivered (%zd bytes, " "partition %" PRId32 ")\n",rkmessage->len, rkmessage->partition);
//     }


// }

void KafkaServiceManager::producer_init(std::string server_name,std::string broker_hostAndPort ){
    //if generated alredy retun it no need to re initialize it .
    if(prod_conf) return;
    if(!prod_conf){
         prod_conf = rd_kafka_conf_new();  
    }

    if (rd_kafka_conf_set(prod_conf, server_name , broker_hostAndPort, errstr,
                          sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%s\n", errstr);
        return 1;
    }

    // before setting rk we call rk to set the delivery report callback
    rd_kafka_conf_set_dr_msg_cb(prod_conf, rd_kafka_delivery_report_callback);

    // Create producer instance
    rk = rd_kafka_new(RD_KAFKA_PRODUCER, prod_conf, errstr, sizeof(errstr));
    if (!rk) {
        fprintf(stderr, "%% Failed to create new producer: %s\n", errstr);
        return 1;
    }

    // delivery report callback
    //rd_kafka_conf_set_dr_msg_cb(prod_conf, rd_kafka_delivery_report_callback);

}

void KafkaServiceManager::produce(string topic , string message, int len){

    //check whether initiated
    if(!prod_conf) return ;

    rd_kafka_resp_err_t err;

    err = rd_kafka_producev(rk,RD_KAFKA_V_TOPIC(topic),RD_KAFKA_V_VALUE((void *)message, len),RD_KAFKA_V_END);

     if (err) {
        fprintf(stderr, "%% Failed to produce to topic %s: %s\n",
                topic, rd_kafka_err2str(err));
              } 
}


