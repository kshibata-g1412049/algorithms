# jump_point_searchのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(jump_point_search_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(jump_point_search jump_point_search.cpp)
target_include_directories(jump_point_search PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_jump_point_search test_jump_point_search.cpp)
target_link_libraries(test_jump_point_search PRIVATE jump_point_search)

add_test(NAME jump_point_search_test COMMAND test_jump_point_search)
