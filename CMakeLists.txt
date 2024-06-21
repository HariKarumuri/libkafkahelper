cmake_minimum_required(VERSION 3.22)

project(mqtthelper VERSION 1.0.0 LANGUAGES CXX)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${READERWRITER_INTERFACE_INCLUDE}
)

add_library(mqtthelper SHARED
    src/mqtthelper.cpp
    src/mqttmanager.cpp
)

target_link_libraries(mqtthelper PRIVATE pthread mosquitto)

set_target_properties(mqtthelper PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(mqtthelper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/inc/mqtthelper.h DESTINATION ${CMAKE_CURRENT_LIST_DIR}/build)
