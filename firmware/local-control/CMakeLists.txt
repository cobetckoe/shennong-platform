cmake_minimum_required(VERSION 3.10)
project(local-control)

file(GLOB_RECURSE SOURCES "src/*.c")

add_library(local-control \)

target_include_directories(local-control 
  PUBLIC \/include
)

if(EXISTS "\/include")
  target_include_directories(local-control PRIVATE \/include)
endif()

target_link_libraries(local-control TuyaOS::SDK)

# 编译选项
target_compile_options(local-control PRIVATE -Wall -Wextra -O2)
