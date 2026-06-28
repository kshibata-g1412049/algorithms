# counting_sortのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(counting_sort_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(counting_sort counting_sort.cpp)
target_include_directories(counting_sort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_counting_sort test_counting_sort.cpp)
target_link_libraries(test_counting_sort PRIVATE counting_sort)

add_test(NAME counting_sort_test COMMAND test_counting_sort)
