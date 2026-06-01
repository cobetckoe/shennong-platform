cmake_minimum_required(VERSION 3.10)
project(duckyclaw)

file(GLOB_RECURSE SOURCES "src/*.c")

add_library(duckyclaw \)

target_include_directories(duckyclaw 
  PUBLIC \/include
)

if(EXISTS "\/include")
  target_include_directories(duckyclaw PRIVATE \/include)
endif()

target_link_libraries(duckyclaw TuyaOS::SDK)

# 编译选项
target_compile_options(duckyclaw PRIVATE -Wall -Wextra -O2)
