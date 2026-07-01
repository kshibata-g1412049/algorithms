# bfsのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(bfs_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(bfs bfs.cpp)
target_include_directories(bfs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_bfs test_bfs.cpp)
target_link_libraries(test_bfs PRIVATE bfs)

add_test(NAME bfs_test COMMAND test_bfs)
