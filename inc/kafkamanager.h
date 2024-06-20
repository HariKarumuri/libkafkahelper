#include <librdkafka/rdkafka.h>


class KafkaServiceManager{
    //constructor
    KafkaServiceManager();
    rd_kafka_t *prod_rk;
    rd_kafka_conf_t *prod_conf;
    rd_kafka_t *cons_rk;        // Consumer instance handle
    rd_kafka_conf_t *cons_conf; // Temporary configuration object


public:
    ~KafkaServiceManager();
    //destructor to free up space , to flush commands 
    static KafkaServiceManager& getInstance();
    void rd_kafka_delivery_report_callback(rd_kafka_t *,const rd_kafka_message_t *, void *)
    void producer_init(std::string ,std::string )
    void produce(string , string , int )

}