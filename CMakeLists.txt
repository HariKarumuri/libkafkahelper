cmake_minimum_required(VERSION 3.22)

project(kafkahelper VERSION 1.0.0 LANGUAGES CXX)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${READERWRITER_INTERFACE_INCLUDE}
)

add_library(kafkahelper SHARED
    src/kafkahelper.cpp
    src/kafkamanager.cpp
)

target_link_libraries(kafkahelper PRIVATE pthread rdkafka)

set_target_properties(kafkahelper PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(kafkahelper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/inc/kafkahelper.h DESTINATION ${CMAKE_CURRENT_LIST_DIR}/build)
