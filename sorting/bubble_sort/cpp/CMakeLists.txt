# bubble_sortのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(bubble_sort_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(bubble_sort bubble_sort.cpp)
target_include_directories(bubble_sort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_bubble_sort test_bubble_sort.cpp)
target_link_libraries(test_bubble_sort PRIVATE bubble_sort)

add_test(NAME bubble_sort_test COMMAND test_bubble_sort)
