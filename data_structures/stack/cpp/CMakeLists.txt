cmake_minimum_required(VERSION 3.10)
project(stack_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(stack stack.cpp)
target_include_directories(stack PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_stack test_stack.cpp)
target_link_libraries(test_stack PRIVATE stack)
add_test(NAME stack_test COMMAND test_stack)
