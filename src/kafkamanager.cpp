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
        /* wait for max 10 seconds */
        rd_kafka_flush(rk, 10 * 1000 );
        // Destroy the producer instance
        rd_kafka_destroy(rk);
    }
}

//making sure to create one instace of this manager and vcan be used by all 
KafkaServiceManager& KafkaServiceManager::getInstance(){
    static KafkaServiceManager obj;
    //created a obj and return it , ensures only one is created 
    return obj;
}


dr_msg_cb()

void producer_init(std::string server_name,std::string brokers ){
    //if generated alredy retun it no need to re initialize it .
    if(prod_conf) return;
    if(!prod_conf){
         prod_conf = rd_kafka_conf_new();  
    }

    if (rd_kafka_conf_set(prod_conf, server_name , brokers, errstr,
                          sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%s\n", errstr);
        return 1;
    }

    // before setting rk we call rk to set the delivery report callback
    rd_kafka_conf_set_dr_msg_cb(prod_conf, dr_msg_cb);

    // Create producer instance
    rk = rd_kafka_new(RD_KAFKA_PRODUCER, prod_conf, errstr, sizeof(errstr));
    if (!rk) {
        fprintf(stderr, "%% Failed to create new producer: %s\n", errstr);
        return 1;
    }

}

