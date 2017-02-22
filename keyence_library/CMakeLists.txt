cmake_minimum_required(VERSION 2.8.3)
project(keyence_comm_lib)

find_package(libsocket REQUIRED)

set(KEYENCE_LIBRARY_INCLUDE_DIRS include ${LIBSOCKET_INCLUDE_DIRS})

# Build the underlying implementation library
add_library(keyence_impl src/impl/keyence_tcp_client.cpp
                         src/impl/messages/high_speed_single_profile.cpp
                         src/impl/ljv7_rawdata.cpp
                         src/impl/messages/get_setting.cpp)

target_include_directories(keyence_impl PUBLIC ${KEYENCE_LIBRARY_INCLUDE_DIRS})

target_link_libraries(keyence_impl ${LIBSOCKET_LIBRARIES})

# Build test program for changing active program
add_executable(keyence_change_program src/change_program.cpp)
target_link_libraries(keyence_change_program keyence_impl)

# Build test program for single keyence profile
add_executable(keyence_get_profile src/get_profile.cpp)
target_link_libraries(keyence_get_profile keyence_impl)

# Build test program for single keyence profile
add_executable(keyence_get_setting src/get_setting.cpp)
target_link_libraries(keyence_get_setting keyence_impl)

