cmake_minimum_required(VERSION 2.8.12)
project(keyence_experimental)

add_compile_options(-std=c++11)

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
  INCLUDE_DIRS
    keyence_library/include
  LIBRARIES
    keyence_impl
  DEPENDS


include_directories(
  keyence_library/include
  ${catkin_INCLUDE_DIRS}
)

add_executable(keyence_driver_node src/keyence_driver_node.cpp)
add_dependencies(keyence_driver_node keyence_driver_generate_messages_cpp)
target_link_libraries(keyence_driver_node
  keyence_impl
  ${catkin_LIBRARIES}
)

add_subdirectory(keyence_library)
