# selection_sortのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(selection_sort_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(selection_sort selection_sort.cpp)
target_include_directories(selection_sort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_selection_sort test_selection_sort.cpp)
target_link_libraries(test_selection_sort PRIVATE selection_sort)

add_test(NAME selection_sort_test COMMAND test_selection_sort)
