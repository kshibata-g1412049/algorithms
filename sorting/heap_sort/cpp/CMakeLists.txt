# heap_sortのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(heap_sort_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(heap_sort heap_sort.cpp)
target_include_directories(heap_sort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_heap_sort test_heap_sort.cpp)
target_link_libraries(test_heap_sort PRIVATE heap_sort)

add_test(NAME heap_sort_test COMMAND test_heap_sort)
