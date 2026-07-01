# linear_searchのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(linear_search_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(linear_search linear_search.cpp)
target_include_directories(linear_search PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_linear_search test_linear_search.cpp)
target_link_libraries(test_linear_search PRIVATE linear_search)

add_test(NAME linear_search_test COMMAND test_linear_search)
