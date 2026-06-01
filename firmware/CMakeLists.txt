cmake_minimum_required(VERSION 3.10)
project(shennong-platform-firmware)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 涂鸦 TuyaOS SDK
find_package(TuyaOS REQUIRED)

# 启用各个模块
enable_language(C)

# 子模块
add_subdirectory(t5-gateway)
add_subdirectory(duckyclaw)
add_subdirectory(meshtastic-client)
add_subdirectory(local-control)
add_subdirectory(edge-inference)
add_subdirectory(storage)
add_subdirectory(ota)

# 编译选项
set(CMAKE_C_FLAGS "\ -Wall -Wextra -O2")
