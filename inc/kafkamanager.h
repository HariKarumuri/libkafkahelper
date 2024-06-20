#include <librdkafka/rdkafka.h>


class KafkaServiceManager{
    KafkaServiceManager();
    rd_kafka_t *rk;
    rd_kafka_conf_t *prod_conf;

    
}