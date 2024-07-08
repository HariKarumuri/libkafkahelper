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

To compare the usage of Kafka without and with the `libkafkahelper` library, I'll provide updated examples for both Kafka consumers and producers. We'll look at the differences in code complexity and ease of use when using the `libkafkahelper` library versus using the default `librdkafka` directly.

### Kafka Consumer Examples

#### Without `libkafkahelper` (using `librdkafka` directly)

```cpp
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <rdkafka.h>

static volatile sig_atomic_t run = 1;

/**
 * Signal termination of program
 */
static void stop(int sig) {
    run = 0;
}

/**
 * Kafka logger callback (optional)
 */
static void logger(const rd_kafka_t *rk, int level, const char *fac, const char *buf) {
    fprintf(stderr, "%% RDKAFKA-%i-%s: %s\n", level, fac, buf);
}

/**
 * Message delivery report callback
 */
static void msg_delivered(rd_kafka_t *rk, const rd_kafka_message_t *rkmessage, void *opaque) {
    if (rkmessage->err) {
        fprintf(stderr, "%% Message delivery failed: %s\n", rd_kafka_err2str(rkmessage->err));
    } else {
        fprintf(stdout, "%% Message delivered (%zd bytes, partition %" PRId32 ")\n",
                rkmessage->len, rkmessage->partition);
    }
}

int main(int argc, char **argv) {
    rd_kafka_t *rk;                        /* Consumer instance handle */
    rd_kafka_conf_t *conf;                 /* Temporary configuration object */
    rd_kafka_topic_partition_list_t *topics; /* Subscribed topics */
    const char *brokers;                   /* Argument: broker list */
    const char *groupid;                   /* Argument: Consumer group id */
    char **topic_list;                     /* Argument: list of topics to subscribe to */
    int topic_cnt;                         /* Number of topics to subscribe to */
    rd_kafka_resp_err_t err;

    if (argc < 4) {
        fprintf(stderr, "%% Usage: %s <broker> <group.id> <topic1> <topic2>..\n", argv[0]);
        return 1;
    }

    brokers = argv[1];
    groupid = argv[2];
    topic_list = &argv[3];
    topic_cnt = argc - 3;

    conf = rd_kafka_conf_new();
    rd_kafka_conf_set_log_cb(conf, logger);

    if (rd_kafka_conf_set(conf, "bootstrap.servers", brokers, NULL, 0) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%% Failed to set broker configuration\n");
        rd_kafka_conf_destroy(conf);
        return 1;
    }

    if (rd_kafka_conf_set(conf, "group.id", groupid, NULL, 0) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%% Failed to set group.id\n");
        rd_kafka_conf_destroy(conf);
        return 1;
    }

    rd_kafka_conf_set_dr_msg_cb(conf, msg_delivered);

    rk = rd_kafka_new(RD_KAFKA_CONSUMER, conf, NULL, 0);
    if (!rk) {
        fprintf(stderr, "%% Failed to create new consumer\n");
        rd_kafka_conf_destroy(conf);
        return 1;
    }

    rd_kafka_poll_set_consumer(rk);

    topics = rd_kafka_topic_partition_list_new(topic_cnt);
    for (int i = 0; i < topic_cnt; i++) {
        rd_kafka_topic_partition_list_add(topics, topic_list[i], RD_KAFKA_PARTITION_UA);
    }

    err = rd_kafka_subscribe(rk, topics);
    if (err) {
        fprintf(stderr, "%% Failed to subscribe to %d topics: %s\n", topics->cnt, rd_kafka_err2str(err));
        rd_kafka_topic_partition_list_destroy(topics);
        rd_kafka_destroy(rk);
        return 1;
    }

    rd_kafka_topic_partition_list_destroy(topics);

    signal(SIGINT, stop);

    while (run) {
        rd_kafka_message_t *rkm = rd_kafka_consumer_poll(rk, 100);
        if (rkm) {
            printf("Message on %s [%" PRId32 "] at offset %" PRId64 ":\n",
                   rd_kafka_topic_name(rkm->rkt), rkm->partition, rkm->offset);
            if (rkm->payload && rkm->len > 0) {
                printf("%.*s\n", (int)rkm->len, (const char *)rkm->payload);
            }
            rd_kafka_message_destroy(rkm);
        }
    }

    fprintf(stderr, "%% Closing consumer\n");
    rd_kafka_consumer_close(rk);
    rd_kafka_destroy(rk);

    return 0;
}
```

#### With `libkafkahelper`

```cpp
#include <stdio.h>
#include <string.h>
#include "kafkahelper.h"

void message_callback(const char* topic, void* req, unsigned int len, void* data) {
    printf("Received message: %s\n", (char*)data);
}

int main() {
    kafka_consumer_init("bootstrap.servers", "localhost:9092", "group.id", "consumer1");

    kafka_set_data_callback(NULL, "test", 1, message_callback);

    while (1) {}

    return 0;
}
```

### Kafka Producer Examples

#### Without `libkafkahelper` (using `librdkafka` directly)

```cpp
#include <stdio.h>
#include <string.h>
#include <librdkafka/rdkafka.h>

void dr_msg_cb(rd_kafka_t *rk, const rd_kafka_message_t *rkmessage, void *opaque) {
    if (rkmessage->err) {
        fprintf(stderr, "%% Message delivery failed: %s\n", rd_kafka_err2str(rkmessage->err));
    } else {
        fprintf(stdout, "%% Message delivered (%zd bytes, partition %" PRId32 ")\n",
                rkmessage->len, rkmessage->partition);
    }
}

int main() {
    rd_kafka_t *rk;
    rd_kafka_conf_t *conf;
    char errstr[512];
    const char *brokers;
    const char *topic;

    brokers = "localhost:9092";
    topic = "test";

    conf = rd_kafka_conf_new();

    if (rd_kafka_conf_set(conf, "bootstrap.servers", brokers, errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%s\n", errstr);
        return 1;
    }

    rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!rk) {
        fprintf(stderr, "%% Failed to create new producer: %s\n", errstr);
        return 1;
    }

    const char *message = "Hello, Kafka!";
    rd_kafka_resp_err_t err;
    err = rd_kafka_producev(
        rk,
        RD_KAFKA_V_TOPIC(topic),
        RD_KAFKA_V_VALUE((void *)message, strlen(message)),
        RD_KAFKA_V_END);

    if (err) {
        fprintf(stderr, "%% Failed to produce to topic %s: %s\n", topic, rd_kafka_err2str(err));
    } else {
        fprintf(stdout, "%% Produced message (%zu bytes)\n", strlen(message));
    }

    rd_kafka_flush(rk, 10 * 1000);

    rd_kafka_destroy(rk);

    return 0;
}
```

#### With `libkafkahelper`

```cpp
#include <stdio.h>
#include <string.h>
#include "kafkahelper.h"

int main() {
    kafka_producer_init("bootstrap.servers", "localhost:9092");

    const char *message = "Hello, Kafka!";
    kafka_produce("test", message, strlen(message));

    return 0;
}
```

### Comparison and Summary

- **Complexity:** Using `libkafkahelper` reduces boilerplate code significantly, especially noticeable in initialization and callback setups.
- **Ease of Use:** The helper library provides simplified functions (`kafka_consumer_init`, `kafka_produce`) that encapsulate more complex operations of `librdkafka`.
- **Functionality:** Both approaches can achieve the same functionality, but `libkafkahelper` abstracts away low-level details, making Kafka integration more straightforward.
- **Error Handling:** Error handling in `libkafkahelper` is usually more encapsulated and may be easier to manage due to the library's design.

In conclusion, `libkafkahelper` enhances productivity by abstracting away low-level Kafka API details, providing a cleaner and more manageable interface for both producers and consumers. This can lead to faster development and easier maintenance of Kafka-based applications.

Certainly! Here's a footer you can use for author Hari Karumuri:

---

**Author:** Hari Karumuri  
**Website:** https://harikarumuri.github.io/Portfolio/ 
**Email:** harikarumuri1086@gmail.com  
**LinkedIn:** - 
