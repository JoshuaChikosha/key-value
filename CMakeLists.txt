cmake_minimum_required(VERSION 3.15)
project(KVStore LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#going to keep existing kv executable
add_executable(kv
  src/main.cpp
  src/kv_store.cpp
  src/command.cpp
)

# Add new server executable
add_executable(kvserver
  src/server_main.cpp
)

# Add new client executable
add_executable(kvclient
  src/client_main.cpp
)

target_include_directories(kv PRIVATE include)
target_include_directories(kvserver PRIVATE include)
target_include_directories(kvclient PRIVATE include)

target_compile_options(kv PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(kvserver PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(kvclient PRIVATE -Wall -Wextra -Wpedantic)