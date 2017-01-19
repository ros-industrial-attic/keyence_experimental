# keyence_experimental
A library for communicating with Keyence LJ-V series sensors over TCP/IP. 

It has been tested on the Keyence LJ-V7080 at Southwest Research Institute.

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

See the [src/utilities](src/utilities) for C++ examples of the above capabilities.