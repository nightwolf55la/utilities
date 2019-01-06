cmake_minimum_required(VERSION 3.5)
project(utilities_ut)

add_executable(utilities_ut
        ut_util.cpp
        ut_util_gen_enumerator.cpp
        ut_util_gen_generator.cpp
        ut_util_gen_range.cpp
        ut_util_gen_zip.cpp
    )
target_link_libraries(utilities_ut utilities)