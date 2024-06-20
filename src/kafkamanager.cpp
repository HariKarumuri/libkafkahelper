#include <iostream>
#include <cstring>
#include "kafkahelper.h"
#include "kafkamanager.h"


//need to setup in parent class
char errstr[512]; 
rd_kafka_t *rk;
rd_kafka_conf_t *prod_conf;


dr_msg_cb()

producer_init(std::string server_name,std::string brokers ){
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

