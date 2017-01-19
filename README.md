# keyence_experimental
A library for communicating with Keyence LJ-V series sensors over TCP/IP. 

It has been tested on the Keyence LJ-V7080 at Southwest Research Institute.

## Organization
This package is divided into two logical components:
 - A cmake-based library for communication with the Keyence (only dependencies are C++-11 and libsocket)
 - A ROS node that uses the library to publish point clouds

## Build
The keyence library requires:
 - A C++11 compiler
 - [Libsocket](https://github.com/dermesser/libsocket)

Building [libsocket](https://github.com/dermesser/libsocket) can be done according to the instructions on their website:

```
cmake CMakeLists.txt
make
make install
```

You can also find an automated build script to do so in the [install-libsocket++.sh](install-libsocket++.sh) file.

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
