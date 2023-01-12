cmake_minimum_required(VERSION 3.24)
project(cpp_json)

set(CMAKE_CXX_STANDARD 14)

include_directories(extlib)
include_directories(include)
include_directories(src)
include_directories(tests)

add_executable(cpp_json
        tests/read/demo1.cpp)
