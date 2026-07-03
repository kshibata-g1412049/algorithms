cmake_minimum_required(VERSION 3.10)
project(doubly_linked_list_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(doubly_linked_list doubly_linked_list.cpp)
target_include_directories(doubly_linked_list PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_doubly_linked_list test_doubly_linked_list.cpp)
target_link_libraries(test_doubly_linked_list PRIVATE doubly_linked_list)
add_test(NAME doubly_linked_list_test COMMAND test_doubly_linked_list)
