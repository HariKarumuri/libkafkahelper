#ifndef __IVC_SERVICE_MANAGER__
#define __IVC_SERVICE_MANAGER__


#include <string>
#include <librdkafka/rdkafka.h>
#include <kafkahelper.h>




class KafkaServiceManager{
    //constructor
    KafkaServiceManager();
    rd_kafka_t *prod_rk;
    rd_kafka_conf_t *prod_conf;
    rd_kafka_t *cons_rk;        // Consumer instance handle
    rd_kafka_conf_t *cons_conf; // Temporary configuration object
    rd_kafka_topic_partition_list_t *topics;
    
    



public:
    ~KafkaServiceManager();
    //destructor to free up space , to flush commands 
    static KafkaServiceManager& getInstance();
    void rd_kafka_delivery_report_callback(rd_kafka_t *,const rd_kafka_message_t *, void *);
    void producer_init(std::string ,std::string );
    void produce(std::string , std::string , int );
    void consumer_init(std::string ,std::string , std::string  , std::string );
    void consume(std::string );

};

#endif 