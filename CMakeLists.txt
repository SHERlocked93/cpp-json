cmake_minimum_required(VERSION 3.24)  # 设置cmake最低版本
project(cpp_json CXX C)   # 项目名称

set(CMAKE_CXX_STANDARD 14)    # 设置C++标准

set(EXTLIB_DIR ./extlib)        # 设置变量
set(SRC_DIR ./src)
set(OBJECT cpp_json)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)  # 指定最终的目标二进制的位置
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)


include_directories(extlib)   # 包含的头文件目录
include_directories(lib)
include_directories(include)
include_directories(src)
include_directories(tests)

#Generate the static library from the library sources
aux_source_directory("extlib/easylogger" DIR_SRCS)
add_library(elog_lib STATIC ${DIR_SRCS})
target_include_directories(elog_lib PUBLIC extlib/easylogger)


# 生成可执行文件
add_executable(OBJECT
        tests/main.cpp
        tests/log_util.cpp
        src/SaveToFile.cpp
        src/ReadFile.cpp
        extlib/cJSON/cJSON.c
        extlib/hsm/hsm.c)

target_link_libraries(OBJECT PRIVATE elog_lib)
