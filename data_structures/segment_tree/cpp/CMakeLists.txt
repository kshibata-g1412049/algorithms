cmake_minimum_required(VERSION 3.10)
project(segment_tree_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(segment_tree segment_tree.cpp)
target_include_directories(segment_tree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_segment_tree test_segment_tree.cpp)
target_link_libraries(test_segment_tree PRIVATE segment_tree)
add_test(NAME segment_tree_test COMMAND test_segment_tree)
