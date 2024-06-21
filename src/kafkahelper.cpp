#include <iostream>
#include <string>
#include "kafkahelper.h"
#include "kafkamanager.h"


void kafka_producer_init(std::string server_name,std::string broker_hostAndPort ){
    KafkaServiceManager::getInstance().producer_init(server_name,broker_hostAndPort);
}
void kafka_produce(std::string topic , std::string message, int len){
    KafkaServiceManager::getInstance().produce(topic,message,len);
}
void kafka_consumer_init(std::string server_name,std::string broker_hostAndPort, std::string group_name , std::string group_id){
    KafkaServiceManager::getInstance().consumer_init(server_name,broker_hostAndPort,group_name,group_id);
}
void kafka_consume(std::string topic){
    KafkaServiceManager::getInstance().consume(topic);
}
