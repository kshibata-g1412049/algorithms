# goal_biased_rrtのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(goal_biased_rrt_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(goal_biased_rrt goal_biased_rrt.cpp)
target_include_directories(goal_biased_rrt PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_goal_biased_rrt test_goal_biased_rrt.cpp)
target_link_libraries(test_goal_biased_rrt PRIVATE goal_biased_rrt)

add_test(NAME goal_biased_rrt_test COMMAND test_goal_biased_rrt)
