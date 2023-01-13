cmake_minimum_required(VERSION 3.24)  # 设置cmake最低版本
project(cpp_json CXX C)   # 项目名称

set(CMAKE_CXX_STANDARD 14)    # 设置C++标准

set(EXTLIB_DIR ./extlib)        # 设置变量
set(SRC_DIR ./src)
set(OBJECT cpp_json)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)  # 指定最终的目标二进制的位置
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(extlib)   # 包含的头文件目录
include_directories(include)
include_directories(src)
include_directories(tests)


# 生成可执行文件
add_executable(OBJECT
        tests/main.cpp extlib/cJSON/cJSON.c src/CreateFile.cpp include/CreateFile.h src/ReadFile.cpp)
