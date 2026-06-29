# fibonacci_searchのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(fibonacci_search_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(fibonacci_search fibonacci_search.cpp)
target_include_directories(fibonacci_search PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_fibonacci_search test_fibonacci_search.cpp)
target_link_libraries(test_fibonacci_search PRIVATE fibonacci_search)

add_test(NAME fibonacci_search_test COMMAND test_fibonacci_search)
