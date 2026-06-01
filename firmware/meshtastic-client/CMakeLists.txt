cmake_minimum_required(VERSION 3.10)
project(meshtastic-client)

file(GLOB_RECURSE SOURCES "src/*.c")

add_library(meshtastic-client \)

target_include_directories(meshtastic-client 
  PUBLIC \/include
)

if(EXISTS "\/include")
  target_include_directories(meshtastic-client PRIVATE \/include)
endif()

target_link_libraries(meshtastic-client TuyaOS::SDK)

# 编译选项
target_compile_options(meshtastic-client PRIVATE -Wall -Wextra -O2)
