## Kafka Helper Library

### Overview

The Kafka Helper Library (`libkafkahelper`) provides a simplified interface for interacting with Apache Kafka, focusing on managing Kafka producers and consumers efficiently. This README outlines the usage and design of the library.

### Singleton Design Pattern

The library employs the Singleton design pattern for `KafkaServiceManager`, ensuring there is only one instance managing Kafka resources across the application. This approach simplifies resource management and provides a global point of access to Kafka operations.

#### `KafkaServiceManager` Class

The `KafkaServiceManager` class encapsulates the initialization and operation of Kafka producers and consumers. Here’s how it integrates with the Singleton pattern:

- **Private Constructor**: The constructor is private, preventing direct instantiation and ensuring that instances are controlled through a static method.
  
- **Static Instance**: The static method `getInstance()` returns the Singleton instance of `KafkaServiceManager`, ensuring that all interactions with Kafka are handled through a single object.
  
- **Lazy Initialization**: The Singleton instance is initialized lazily, meaning it is created the first time `getInstance()` is called, and subsequent calls return the existing instance.

### Usage Example

Here’s a simple example demonstrating how to use `KafkaServiceManager` in your application:

```cpp
#include <iostream>
#include "kafkamanager.h"

int main() {
    // Initialize Kafka producer and consumer
    KafkaServiceManager::getInstance().producer_init("server_name", "broker_hostAndPort");
    KafkaServiceManager::getInstance().consumer_init("server_name", "broker_hostAndPort", "group_name", "group_id");

    // Produce a message
    KafkaServiceManager::getInstance().produce("topic_name", "Hello Kafka!", strlen("Hello Kafka!"));

    // Consume messages from a topic
    KafkaServiceManager::getInstance().consume("topic_name");

    // No need to manually destroy KafkaServiceManager - handled by Singleton destructor

    return 0;
}
```

### Documentation for `KafkaServiceManager`

#### Class: `KafkaServiceManager`

##### Purpose

The `KafkaServiceManager` class manages Kafka producers and consumers as a Singleton instance, ensuring efficient resource utilization and global access to Kafka operations.

##### Public Methods

- `static KafkaServiceManager& getInstance()`
  - Returns the Singleton instance of `KafkaServiceManager`.
  
- `void producer_init(std::string server_name, std::string broker_hostAndPort)`
  - Initializes a Kafka producer with the specified server and broker information.
  
- `void produce(std::string topic, std::string message, int len)`
  - Sends a message to the specified Kafka topic using the initialized producer.
  
- `void consumer_init(std::string server_name, std::string broker_hostAndPort, std::string group_name, std::string group_id)`
  - Initializes a Kafka consumer with the specified server, broker, consumer group name, and group ID.
  
- `void consume(std::string topic)`
  - Starts consuming messages from the specified Kafka topic using the initialized consumer.

##### Private Members

- `rd_kafka_t *prod_rk`
  - Pointer to the Kafka producer instance (`rd_kafka_t`).

- `rd_kafka_conf_t *prod_conf`
  - Pointer to the Kafka producer configuration (`rd_kafka_conf_t`).

- `rd_kafka_t *cons_rk`
  - Pointer to the Kafka consumer instance (`rd_kafka_t`).

- `rd_kafka_conf_t *cons_conf`
  - Pointer to the Kafka consumer configuration (`rd_kafka_conf_t`).

- `rd_kafka_topic_partition_list_t *topics`
  - Pointer to the list of Kafka topics for consumer subscription (`rd_kafka_topic_partition_list_t`).

##### Notes

- Ensure proper initialization and error handling in production code.
- The Singleton pattern simplifies resource management and access to Kafka functionality across the application.

