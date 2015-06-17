cmake_minimum_required(VERSION 2.8.12)
project(keyence_experimental)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  pcl_ros
)

catkin_package(
  INCLUDE_DIRS
    keyence_library/include
  LIBRARIES
    keyence_impl
  DEPENDS
)

include_directories(
  include
  keyence_library/include
  ${catkin_INCLUDE_DIRS}
)

add_executable(keyence_driver_node src/keyence_driver_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
add_dependencies(keyence_driver_node keyence_driver_generate_messages_cpp)

## Specify libraries to link a library or executable target against
target_link_libraries(keyence_driver_node
  keyence_impl
  ${catkin_LIBRARIES}
)

add_subdirectory(keyence_library)
