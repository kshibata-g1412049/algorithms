# ida_starのcpp実装。このフォルダだけを取り出しても
# `mkdir build && cd build && cmake .. && make && ctest` でビルド・テストできる
# ように、project()から自己完結させている。
cmake_minimum_required(VERSION 3.10)
project(ida_star_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

add_library(ida_star ida_star.cpp)
target_include_directories(ida_star PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_ida_star test_ida_star.cpp)
target_link_libraries(test_ida_star PRIVATE ida_star)

add_test(NAME ida_star_test COMMAND test_ida_star)
