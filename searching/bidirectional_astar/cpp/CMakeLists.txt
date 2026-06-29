# bidirectional_astarのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(bidirectional_astar_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(bidirectional_astar bidirectional_astar.cpp)
target_include_directories(bidirectional_astar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_bidirectional_astar test_bidirectional_astar.cpp)
target_link_libraries(test_bidirectional_astar PRIVATE bidirectional_astar)

add_test(NAME bidirectional_astar_test COMMAND test_bidirectional_astar)
