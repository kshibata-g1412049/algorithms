cmake_minimum_required(VERSION 3.10)
project(circular_buffer_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(circular_buffer circular_buffer.cpp)
target_include_directories(circular_buffer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_circular_buffer test_circular_buffer.cpp)
target_link_libraries(test_circular_buffer PRIVATE circular_buffer)
add_test(NAME circular_buffer_test COMMAND test_circular_buffer)
