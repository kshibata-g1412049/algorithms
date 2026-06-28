# merge_sortのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(merge_sort_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(merge_sort merge_sort.cpp)
target_include_directories(merge_sort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_merge_sort test_merge_sort.cpp)
target_link_libraries(test_merge_sort PRIVATE merge_sort)

add_test(NAME merge_sort_test COMMAND test_merge_sort)
