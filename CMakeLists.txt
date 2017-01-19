cmake_minimum_required(VERSION 2.8.12)
project(keyence_experimental)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  pcl_ros
  message_generation
)

add_service_files(
  FILES
  ChangeProgram.srv
)

generate_messages(
  DEPENDENCIES
)

catkin_package(
  INCLUDE_DIRS
    keyence_library/include
  LIBRARIES
    keyence_impl
  CATKIN_DEPENDS
    message_runtime
  DEPENDS
)

include_directories(
  keyence_library/include
  ${catkin_INCLUDE_DIRS}
)

add_executable(keyence_driver_node src/keyence_driver_node.cpp)
add_dependencies(keyence_driver_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(keyence_driver_node
  keyence_impl
  ${catkin_LIBRARIES}
)

add_subdirectory(keyence_library)
