# dijkstraのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(dijkstra_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(dijkstra dijkstra.cpp)
target_include_directories(dijkstra PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_dijkstra test_dijkstra.cpp)
target_link_libraries(test_dijkstra PRIVATE dijkstra)

add_test(NAME dijkstra_test COMMAND test_dijkstra)
