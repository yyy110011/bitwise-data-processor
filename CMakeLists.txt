cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 14)
project(data_processor C CXX)

add_library(logging SHARED
  logging/src/log.c
)

add_executable(data_processor 
    main.cc 
    src/data_worker.cc
    src/data_analyzer.cc
    src/data_sender.cc
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(data_processor curl)
target_link_libraries(data_processor pthread)
target_link_libraries(data_processor logging)

add_executable(small_test 
    small_test.cc 
    src/data_worker.cc
    src/data_analyzer.cc
    src/data_sender.cc
)
find_package(GTest REQUIRED)
target_link_libraries(small_test GTest::GTest GTest::Main)
target_link_libraries(small_test curl)
target_link_libraries(small_test pthread)
target_link_libraries(small_test logging)