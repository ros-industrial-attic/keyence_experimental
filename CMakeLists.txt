cmake_minimum_required(VERSION 2.8.12)
project(keyence_experimental)
add_compile_options(-std=c++11 -Wall -Wextra)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  message_generation
  message_runtime
  pcl_ros
  roscpp
  std_msgs
)

## Driver makes use of the libsocket libraries
find_package(libsocket REQUIRED)

################################################
## Declare ROS messages, services and actions ##
################################################

add_service_files(
  FILES
  ChangeProgram.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

###################################
## catkin specific configuration ##
###################################

catkin_package(
  INCLUDE_DIRS
  include
  LIBRARIES
  keyence_impl
  CATKIN_DEPENDS
  message_generation
  message_runtime
  pcl_ros
  std_msgs
  DEPENDS
  libsocket
)

###########
## Build ##
###########

include_directories(
  include
  ${libsocket_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
)

add_executable(
  keyence_driver_node
  src/keyence_driver_node.cpp
)
add_dependencies(
  keyence_driver_node
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)
target_link_libraries(
  keyence_driver_node
  keyence_impl
  ${catkin_LIBRARIES}
)

# Build the underlying implementation library
add_library(
  keyence_impl
  src/impl/keyence_tcp_client.cpp
  src/impl/ljv7_rawdata.cpp
  src/impl/messages/get_setting.cpp
  src/impl/messages/high_speed_single_profile.cpp
)
target_link_libraries(
  keyence_impl
  ${libsocket_LIBRARIES}
)

# Build test program for changing active program
add_executable(
  keyence_change_program
  src/change_program.cpp
)
target_link_libraries(
  keyence_change_program
  keyence_impl
)

# Build test program for single keyence profile
add_executable(
  keyence_get_profile
  src/get_profile.cpp
)
target_link_libraries(
  keyence_get_profile
  keyence_impl
)

# Build test program for single keyence profile
add_executable(
  keyence_get_setting
  src/get_setting.cpp
)
target_link_libraries(
  keyence_get_setting
  keyence_impl
)

#############
## Install ##
#############

## Mark executables and/or libraries for installation
install(
  TARGETS
  keyence_change_program
  keyence_driver_node
  keyence_get_profile
  keyence_get_setting
  keyence_impl
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

## Mark cpp header files for installation
install(
  DIRECTORY
  include/${PROJECT_NAME}/
  DESTINATION
  ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

## Mark other files for installation (e.g. launch and bag files, etc.)
install(
  DIRECTORY
  config
  DESTINATION
  ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(
  DIRECTORY 
  launch 
  DESTINATION
  ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

#############
## Testing ##
#############
