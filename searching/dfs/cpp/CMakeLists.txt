# dfsのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(dfs_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(dfs dfs.cpp)
target_include_directories(dfs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_dfs test_dfs.cpp)
target_link_libraries(test_dfs PRIVATE dfs)

add_test(NAME dfs_test COMMAND test_dfs)
