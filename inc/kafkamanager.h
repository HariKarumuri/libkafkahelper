#include <librdkafka/rdkafka.h>


class KafkaServiceManager{
    //constructor
    KafkaServiceManager();
    rd_kafka_t *rk;
    rd_kafka_conf_t *prod_conf;
public:
    ~KafkaServiceManager();
    //destructor to free up space , to flush commands 
    static KafkaServiceManager& getInstance();


}