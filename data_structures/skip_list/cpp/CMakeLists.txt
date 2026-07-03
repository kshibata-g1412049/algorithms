cmake_minimum_required(VERSION 3.10)
project(skip_list_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(skip_list skip_list.cpp)
target_include_directories(skip_list PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_skip_list test_skip_list.cpp)
target_link_libraries(test_skip_list PRIVATE skip_list)
add_test(NAME skip_list_test COMMAND test_skip_list)
