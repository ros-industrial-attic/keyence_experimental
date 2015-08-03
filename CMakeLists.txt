cmake_minimum_required(VERSION 2.8.3)
project(keyence_experimental)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  pcl_ros
)

add_service_files(
  FILES
  ChangeProgram.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs  # Or other packages containing msgs
)

catkin_package(
 CATKIN_DEPENDS roscpp pcl_ros
 LIBRARIES keyence_impl
 INCLUDE_DIRS keyence_library/include
)

###########
## Build ##
###########

add_subdirectory(keyence_library EXCLUDE_FROM_ALL)

include_directories(
  include
  keyence_library/include
  ${catkin_INCLUDE_DIRS}
)

add_executable(keyence_driver_node src/keyence_driver_node.cpp)
add_dependencies(keyence_driver_node keyence_driver_generate_messages_cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(keyence_driver_node
  keyence_impl
  ${catkin_LIBRARIES}
)
