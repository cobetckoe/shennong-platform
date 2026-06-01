cmake_minimum_required(VERSION 3.10)
project(storage)

file(GLOB_RECURSE SOURCES "src/*.c")

add_library(storage \)

target_include_directories(storage 
  PUBLIC \/include
)

if(EXISTS "\/include")
  target_include_directories(storage PRIVATE \/include)
endif()

target_link_libraries(storage TuyaOS::SDK)

# 编译选项
target_compile_options(storage PRIVATE -Wall -Wextra -O2)
