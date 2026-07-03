cmake_minimum_required(VERSION 3.10)
project(trie_cpp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(trie trie.cpp)
target_include_directories(trie PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_executable(test_trie test_trie.cpp)
target_link_libraries(test_trie PRIVATE trie)
add_test(NAME trie_test COMMAND test_trie)
