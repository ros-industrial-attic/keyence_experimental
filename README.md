[![Build Status](https://travis-ci.org/ros-industrial/keyence_experimental.svg?branch=kinetic-devel)](https://travis-ci.org/ros-industrial/keyence_experimental)
# keyence_experimental
A library for communicating with Keyence LJ-V series sensors over TCP/IP. 

It has been tested on the Keyence LJ-V7080 at Southwest Research Institute.

## Organization
This package is divided into two logical components:
 - A cmake-based library for communication with the Keyence (only dependencies are C++-11 and libsocket)
 - A ROS node that uses the library to publish point clouds

## Build
The keyence library requires:
 - [wstool](http://wiki.ros.org/wstool)

Download wstool and dependencies:
```
mkdir -p my_workspace/src
cd my_workspace/src
wstool init .
wstool merge https://github.com/ros-industrial/keyence_experimental/raw/kinetic-devel/keyence_experimental.rosinstall
wstool update
cd ..
catkin build
```

## Capability
Currently, laser program configuration must be done through the Windows-based Keyence utility. This API supports:
  - Changing active programs
  - Sampling individual profiles via the service interface

## Examples

See the [keyence_library/src/](keyence_library/src/) directory for C++ examples of the above capabilities.

## ROS Node
To run the ros node:
```
rosrun keyence_experimental keyence_driver_node _controller_ip:=xxx.xxx.xxx.xxx
```

Or set the `controller_ip` parameter as a private param in a launch file.

The following additional parameters are supported:
  - `controller_port`, defaults to 24691, the service-port of the Keyence controller
  - `frame_id`, defaults to `sensor_optical_frame`, the frame of reference included in the published point cloud

By default the node publishes XYZ point clouds on the `profiles` topic. Note that nothing will be published if
no one subscribes that topic.

The active program can be set via the `keyence_experimental::ChangeProgram` service call. See the [srv/](srv/) directory for more info.
