cmake_minimum_required(VERSION 2.8.3)
project(keyence_experimental)

include_directories(include)

# Build the underlying implementation library
add_library(keyence_impl src/impl/keyence_tcp_client.cpp
                         # messages
                         src/impl/messages/high_speed_single_profile.cpp
                         src/impl/ljv7_rawdata.cpp
                         src/impl/messages/prepare_high_speed.cpp
                         src/impl/messages/start_high_speed.cpp)

target_link_libraries(keyence_impl socket++)

# Build test program for changing active program
add_executable(keyence_change_program src/utilities/change_program.cpp)
target_link_libraries(keyence_change_program keyence_impl)

# Build test program for single keyence profile
add_executable(keyence_get_profile src/utilities/get_profile.cpp)
target_link_libraries(keyence_get_profile keyence_impl)
