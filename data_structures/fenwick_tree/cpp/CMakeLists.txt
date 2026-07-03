cmake_minimum_required(VERSION 3.10)
project(fenwick_tree_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(fenwick_tree fenwick_tree.cpp)
target_include_directories(fenwick_tree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_fenwick_tree test_fenwick_tree.cpp)
target_link_libraries(test_fenwick_tree PRIVATE fenwick_tree)
add_test(NAME fenwick_tree_test COMMAND test_fenwick_tree)
