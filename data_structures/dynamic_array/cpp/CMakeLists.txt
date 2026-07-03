cmake_minimum_required(VERSION 3.10)
project(dynamic_array_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(dynamic_array dynamic_array.cpp)
target_include_directories(dynamic_array PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_dynamic_array test_dynamic_array.cpp)
target_link_libraries(test_dynamic_array PRIVATE dynamic_array)
add_test(NAME dynamic_array_test COMMAND test_dynamic_array)
