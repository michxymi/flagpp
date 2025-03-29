# FlagPlusPlus (flagpp)

A modern, header-only C++ feature flag library with zero dependencies.

## Features

- **Header-only**: Just include and use
- **Zero dependencies**: Uses only the C++ standard library
- **Thread-safe**: Safe for concurrent access
- **Type-safe**: Strong typing for flag values
- **Modern C++**: Requires C++17 or later
- **Minimal overhead**: Optimized for performance
- **Intuitive API**: Simple and expressive interface

## Requirements

- C++17 or later
- CMake 3.14 or later (for building examples, tests, and benchmarks)

## Installation

### Option 1: Copy the Header

Simply copy `include/flagpp.h` to your project and include it.

### Option 2: CMake Integration

```cmake
# Add as a subdirectory
add_subdirectory(path/to/flagplusplus)
target_link_libraries(your_target PRIVATE flagplusplus)
