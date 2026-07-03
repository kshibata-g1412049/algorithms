cmake_minimum_required(VERSION 3.10)
project(union_find_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(union_find union_find.cpp)
target_include_directories(union_find PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_union_find test_union_find.cpp)
target_link_libraries(test_union_find PRIVATE union_find)
add_test(NAME union_find_test COMMAND test_union_find)
