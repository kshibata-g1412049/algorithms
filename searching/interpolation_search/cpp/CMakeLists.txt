# interpolation_searchのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(interpolation_search_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(interpolation_search interpolation_search.cpp)
target_include_directories(interpolation_search PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_interpolation_search test_interpolation_search.cpp)
target_link_libraries(test_interpolation_search PRIVATE interpolation_search)

add_test(NAME interpolation_search_test COMMAND test_interpolation_search)
