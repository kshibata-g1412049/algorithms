# informed_rrt_starのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(informed_rrt_star_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(informed_rrt_star informed_rrt_star.cpp)
target_include_directories(informed_rrt_star PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_informed_rrt_star test_informed_rrt_star.cpp)
target_link_libraries(test_informed_rrt_star PRIVATE informed_rrt_star)

add_test(NAME informed_rrt_star_test COMMAND test_informed_rrt_star)
