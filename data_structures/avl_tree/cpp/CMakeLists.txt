cmake_minimum_required(VERSION 3.10)
project(avl_tree_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(avl_tree avl_tree.cpp)
target_include_directories(avl_tree PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_avl_tree test_avl_tree.cpp)
target_link_libraries(test_avl_tree PRIVATE avl_tree)
add_test(NAME avl_tree_test COMMAND test_avl_tree)
