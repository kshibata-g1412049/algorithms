# rrt_connectのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(rrt_connect_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(rrt_connect rrt_connect.cpp)
target_include_directories(rrt_connect PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_rrt_connect test_rrt_connect.cpp)
target_link_libraries(test_rrt_connect PRIVATE rrt_connect)

add_test(NAME rrt_connect_test COMMAND test_rrt_connect)
