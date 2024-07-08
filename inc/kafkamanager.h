#ifndef __IVC_SERVICE_MANAGER__
#define __IVC_SERVICE_MANAGER__


#include <string>
#include <thread>
#include <rdkafka.h>
#include <kafkahelper.h>
#include <unordered_map>




class KafkaServiceManager{
    //constructor
    KafkaServiceManager();
    rd_kafka_t *prod_rk;
    rd_kafka_conf_t *prod_conf;
    rd_kafka_t *cons_rk;        // Consumer instance handle
    rd_kafka_conf_t *cons_conf; // Temporary configuration object
    rd_kafka_topic_partition_list_t *topics;
    std::unordered_map<std::string, std::pair<callback_, void*>> kTopicCbMap;
    std::thread consumerThread;
    bool kRunning;
    
    

    void consume_messages();
    void stop_consumer_thread();

    static void signal_handler(int sig);


public:
    ~KafkaServiceManager();
    //destructor to free up space , to flush commands 
    static KafkaServiceManager& getInstance();
    void producer_init(std::string ,std::string );
    void produce(std::string , void* , int );
    void consumer_init(std::string ,std::string , std::string  , std::string );
    void set_consumer_callback(void* , std::string , int , callback_ );

};

#endif 