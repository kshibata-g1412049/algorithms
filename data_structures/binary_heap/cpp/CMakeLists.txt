cmake_minimum_required(VERSION 3.10)
project(binary_heap_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(binary_heap binary_heap.cpp)
target_include_directories(binary_heap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_binary_heap test_binary_heap.cpp)
target_link_libraries(test_binary_heap PRIVATE binary_heap)
add_test(NAME binary_heap_test COMMAND test_binary_heap)
