# exponential_searchのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(exponential_search_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(exponential_search exponential_search.cpp)
target_include_directories(exponential_search PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_exponential_search test_exponential_search.cpp)
target_link_libraries(test_exponential_search PRIVATE exponential_search)

add_test(NAME exponential_search_test COMMAND test_exponential_search)
