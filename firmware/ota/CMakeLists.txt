cmake_minimum_required(VERSION 3.10)
project(ota)

file(GLOB_RECURSE SOURCES "src/*.c")

add_library(ota \)

target_include_directories(ota 
  PUBLIC \/include
)

if(EXISTS "\/include")
  target_include_directories(ota PRIVATE \/include)
endif()

target_link_libraries(ota TuyaOS::SDK)

# 编译选项
target_compile_options(ota PRIVATE -Wall -Wextra -O2)
