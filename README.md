# WardenSTL - Simple header-only C++ Standard Template Library
This library is specially created for occasions where there is no standard library and where resources are limited. It implements the basic functions from the standard library and some that are useful but not present 
in it. It aims to support C++98/03 and bring some functions from newer versions, and implement as many features from newer versions as possible. 

## Main Features
 - No dynamic memory allocation
 - No RTTI
 - Fixed size containers (in progress)
 - CRC calculations (will be added soon)
 - Checksum & hash functions (in progress)
 - Error handling
 - Independence from standard library
 - Simple low-level allocator support (`Allocator` interface, `BumpAllocator`)
 - Advanced and thoughtful container API that separates storage from the container logic
 - Compile-time features
    * Rich traits/meta-programming infrastructure
    * Strong constexpr support
 - Templates for buffers, streams (will be added soon)
 - Very small usage of virtual functions. They are used only if they are really needed
 - Doxygen generated documentation
 - Header-only implementation 

## Installation
There is nothing special. Just copy or clone the GitHub project into appropriate directory. All headers are located in `include` folder.

## Generating documentation & testing
You should have [SCons](https://scons.org/) installed. Issue the `scons` command to generate documentation. To run test, execute `scons test`.  To clean everything, just run `scons -c` command.

## Acknowledgments
This library was inspired by the [Embedded Template Library (ETL)](https://github.com/ETLCPP/etl), and some parts of the code were adapted from it.
