cmake_minimum_required(VERSION 3.22)

project(kafkahelper VERSION 1.0.0 LANGUAGES CXX)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${READERWRITER_INTERFACE_INCLUDE}
)

# Kafka LIB Path
find_library(RDKAFKA_CORE_LIBRARY
  NAMES rdkafka
  PATHS ${CMAKE_LIBRARY_PATH}
  NO_DEFAULT_PATH
)

# Kafka INCLUDE Path
include_directories(${CMAKE_INCLUDE_PATH})

if(RDKAFKA_CORE_LIBRARY)
  get_filename_component(RDKAFKA_CORE_PATH ${RDKAFKA_CORE_LIBRARY} DIRECTORY)
  message(STATUS "RDKAFKA library found at ${RDKAFKA_CORE_PATH}")
else()
  message(FATAL_ERROR "RDKAFKA library not found")
endif()


add_library(kafkahelper SHARED
    src/kafkahelper.cpp
    src/kafkamanager.cpp
)

target_link_libraries(kafkahelper PRIVATE pthread ${RDKAFKA_CORE_LIBRARY})

set_target_properties(kafkahelper PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_include_directories(kafkahelper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(kafkahelper PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/inc/kafkahelper.h DESTINATION ${CMAKE_BINARY_DIR}/inc)

install(TARGETS kafkahelper
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin

)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/inc/kafkahelper.h
  DESTINATION include
)

message("CMAKE CONFIGURATION FOR KAFKAHELPER IS SUCCESSFULL.....")