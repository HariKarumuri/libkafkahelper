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
EXPORT void kafka_producer_init(const char* ,const char*  );
EXPORT void kafka_produce(const char* , const char* , int );
EXPORT void kafka_consumer_init(const char*,const char*,const char*, const char*);
EXPORT void kafka_consume(const char*);

#ifdef __cplusplus
}
#endif

#endif // KAFKA_HELPER_H