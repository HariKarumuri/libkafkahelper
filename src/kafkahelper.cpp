#include <iostream>
#include <string>
#include "kafkahelper.h"
#include "kafkamanager.h"


void kafka_producer_init(const char* server_name,const char* broker_hostAndPort ){
    KafkaServiceManager::getInstance().producer_init(server_name,broker_hostAndPort);
}
void kafka_produce(const char* topic , const char* message, int len){
    KafkaServiceManager::getInstance().produce(topic,message,len);
}
void kafka_consumer_init(const char* server_name,const char* broker_hostAndPort, const char* group_name , const char* group_id){
    KafkaServiceManager::getInstance().consumer_init(server_name,broker_hostAndPort,group_name,group_id);
}
void kafka_set_data_callback(void* req, const char* topic, int partition_size, callback_ cb){
    KafkaServiceManager::getInstance().set_consumer_callback(req,topic,partition_size , cb);
}
