# weighted_astarのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(weighted_astar_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(weighted_astar weighted_astar.cpp)
target_include_directories(weighted_astar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_weighted_astar test_weighted_astar.cpp)
target_link_libraries(test_weighted_astar PRIVATE weighted_astar)

add_test(NAME weighted_astar_test COMMAND test_weighted_astar)
