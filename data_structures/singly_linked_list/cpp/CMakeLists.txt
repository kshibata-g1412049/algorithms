cmake_minimum_required(VERSION 3.10)
project(singly_linked_list_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(singly_linked_list singly_linked_list.cpp)
target_include_directories(singly_linked_list PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_singly_linked_list test_singly_linked_list.cpp)
target_link_libraries(test_singly_linked_list PRIVATE singly_linked_list)
add_test(NAME singly_linked_list_test COMMAND test_singly_linked_list)
