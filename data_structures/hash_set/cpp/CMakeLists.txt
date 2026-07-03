cmake_minimum_required(VERSION 3.10)
project(hash_set_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(hash_set hash_set.cpp)
target_include_directories(hash_set PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_hash_set test_hash_set.cpp)
target_link_libraries(test_hash_set PRIVATE hash_set)
add_test(NAME hash_set_test COMMAND test_hash_set)
