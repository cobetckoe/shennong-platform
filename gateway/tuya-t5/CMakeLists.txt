cmake_minimum_required(VERSION 3.10)
project(t5-gateway)

add_library(t5-gateway
  src/main.c
  src/storage.c
  src/model_manager.c
  src/data_manager.c
  src/bio_config.c
)

target_include_directories(t5-gateway PUBLIC include)
target_link_libraries(t5-gateway TuyaOS::SDK)