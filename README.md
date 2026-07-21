# WardenSTL - Simple header-only C++ Standard Template Library

This library is specially created for occasions where there is no standard library and where resources are limited. It implements the basic functions from the standard library and some that are useful but not present in it. It aims to support C++98/03 and bring some functions from newer versions, and implement as many features from newer versions as possible.

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
  - Rich traits/meta-programming infrastructure
  - Strong constexpr support
- Templates for buffers, streams (will be added soon)
- Very small usage of virtual functions. They are used only if they are really needed
- Doxygen generated documentation
- Header-only implementation

## Installation

There is nothing special. Just copy or clone the GitHub project into appropriate directory. All headers are located in `include` folder.

## Development Setup

Run setup script located at `scripts` folder to setup Python virtual environment (needed for `SCons`).

For Linux/MacOS:

```sh
./scripts/setup.sh
```

For Windows:

```psh
./scripts/setup.ps1
```

If you're using VS Code, select `.venv` as the Python interpreter. New integrated terminals will automatically activate the environment. Or you can run `Setup Development Environment` task instead.

## Building, testing & generating documentation

> [!IMPORTANT]
> To able to generate documentation, you must have `doxygen` installed

Everything is done using `SCons`. To build the library, run:

```sh
scons
```

This will build the library and run all tests. To generate documentation, run:

```sh
scons docs
```

To execute tests only, run:

```sh
scons run_test
```

## Acknowledgments

This library was inspired by the [Embedded Template Library (ETL)](https://github.com/ETLCPP/etl), and some parts of the code were adapted from it.
