cmake_minimum_required(VERSION 3.10)
project(hash_map_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(hash_map hash_map.cpp)
target_include_directories(hash_map PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_hash_map test_hash_map.cpp)
target_link_libraries(test_hash_map PRIVATE hash_map)
add_test(NAME hash_map_test COMMAND test_hash_map)
