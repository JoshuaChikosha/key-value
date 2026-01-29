cmake_minimum_required(VERSION 3.15)
project(KVStore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(kv
  src/main.cpp
  src/kv_store.cpp
  src/command.cpp
)

target_include_directories(kv PRIVATE include)

target_compile_options(kv PRIVATE -Wall -Wextra -Wpedantic)
