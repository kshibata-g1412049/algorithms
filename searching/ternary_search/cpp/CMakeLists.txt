# ternary_searchのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(ternary_search_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ternary_search ternary_search.cpp)
target_include_directories(ternary_search PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_ternary_search test_ternary_search.cpp)
target_link_libraries(test_ternary_search PRIVATE ternary_search)

add_test(NAME ternary_search_test COMMAND test_ternary_search)
