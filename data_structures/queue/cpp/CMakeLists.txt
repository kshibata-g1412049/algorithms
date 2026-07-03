cmake_minimum_required(VERSION 3.10)
project(queue_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(queue_ds queue.cpp)
target_include_directories(queue_ds PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_queue test_queue.cpp)
target_link_libraries(test_queue PRIVATE queue_ds)
add_test(NAME queue_test COMMAND test_queue)
