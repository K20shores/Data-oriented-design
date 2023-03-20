cmake_minimum_required(VERSION 3.21)

project(
  data-oriented-design
  VERSION 0.0.0
  LANGUAGES CXX
)

# C++ options
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# dependencies
include(cmake/plotter_target.cmake)

# executables
add_subdirectory(contiguous_arrays)