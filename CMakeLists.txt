cmake_minimum_required(VERSION 2.8.3)
project(keyence_experimental)

# add_definitions(-std=c++11)

include_directories(include)

add_executable(keyence_test src/impl/keyence_tcp_client.cpp
                            src/test_main.cpp)

target_link_libraries(keyence_test socket++)