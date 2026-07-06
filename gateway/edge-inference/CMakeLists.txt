cmake_minimum_required(VERSION 3.10)
project(edge-inference)

file(GLOB_RECURSE SOURCES "src/*.c")

add_library(edge-inference \)

target_include_directories(edge-inference 
  PUBLIC \/include
)

if(EXISTS "\/include")
  target_include_directories(edge-inference PRIVATE \/include)
endif()

target_link_libraries(edge-inference TuyaOS::SDK)

# 编译选项
target_compile_options(edge-inference PRIVATE -Wall -Wextra -O2)
