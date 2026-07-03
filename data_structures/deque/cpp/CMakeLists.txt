cmake_minimum_required(VERSION 3.10)
project(deque_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(deque_ds deque.cpp)
target_include_directories(deque_ds PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_deque test_deque.cpp)
target_link_libraries(test_deque PRIVATE deque_ds)
add_test(NAME deque_test COMMAND test_deque)
