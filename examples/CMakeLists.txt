cmake_minimum_required(VERSION 3.5)
project(utilities_example)

add_executable(utilities_example
        util_gen.cpp
    )
target_link_libraries(utilities_example utilities)