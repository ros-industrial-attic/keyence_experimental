###########
## Build ##
###########

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
  keyence_get_profile
  keyence_get_setting
  keyence_impl
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)
