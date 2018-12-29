cmake_minimum_required(VERSION 3.5)
project(utilities)

set(CMAKE_CXX_STANDARD 17)

add_library(utilities INTERFACE)
target_include_directories(utilities INTERFACE include/)

include_directories(Catch2/single_include)
add_subdirectory(unittests)

