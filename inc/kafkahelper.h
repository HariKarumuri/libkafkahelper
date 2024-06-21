#ifndef KAFKA_HELPER_H
#define KAFKA_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

//here functions 
EXPORT void kafka_producer_init(char * ,char *  );
EXPORT void kafka_produce(char * , char * , int );
EXPORT void kafka_consumer_init(char *,char *,char *, char *);
EXPORT void kafka_consume(char *);

#ifdef __cplusplus
}
#endif

#endif // KAFKA_HELPER_H