cmake_minimum_required(VERSION 2.8.3)
project(keyence_experimental)

include_directories(include)

# Build the underlying implementation library
add_library(keyence_impl src/impl/keyence_tcp_client.cpp
                         src/impl/ljv7_rawdata.cpp)
target_link_libraries(keyence_impl socket++)


# Build test program for changing active program
add_executable(keyence_change_program src/change_program.cpp)
target_link_libraries(keyence_change_program keyence_impl)
