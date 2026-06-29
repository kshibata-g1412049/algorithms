# rrt_starのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(rrt_star_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(rrt_star rrt_star.cpp)
target_include_directories(rrt_star PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_rrt_star test_rrt_star.cpp)
target_link_libraries(test_rrt_star PRIVATE rrt_star)

add_test(NAME rrt_star_test COMMAND test_rrt_star)
