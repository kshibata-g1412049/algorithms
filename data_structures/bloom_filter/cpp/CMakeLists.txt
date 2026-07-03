cmake_minimum_required(VERSION 3.10)
project(bloom_filter_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(bloom_filter bloom_filter.cpp)
target_include_directories(bloom_filter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_bloom_filter test_bloom_filter.cpp)
target_link_libraries(test_bloom_filter PRIVATE bloom_filter)
add_test(NAME bloom_filter_test COMMAND test_bloom_filter)
