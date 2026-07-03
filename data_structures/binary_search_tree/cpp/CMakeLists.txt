cmake_minimum_required(VERSION 3.10)
project(binary_search_tree_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(binary_search_tree binary_search_tree.cpp)
target_include_directories(binary_search_tree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_binary_search_tree test_binary_search_tree.cpp)
target_link_libraries(test_binary_search_tree PRIVATE binary_search_tree)
add_test(NAME binary_search_tree_test COMMAND test_binary_search_tree)
