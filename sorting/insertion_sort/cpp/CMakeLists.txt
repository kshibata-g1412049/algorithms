# insertion_sortのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(insertion_sort_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(insertion_sort insertion_sort.cpp)
target_include_directories(insertion_sort PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_insertion_sort test_insertion_sort.cpp)
target_link_libraries(test_insertion_sort PRIVATE insertion_sort)

add_test(NAME insertion_sort_test COMMAND test_insertion_sort)
