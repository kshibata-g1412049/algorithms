# ルートCMakeLists.txt
#
# このファイルはリポジトリ内の全アルゴリズムのC++実装をまとめてビルド・テストするための
# 便宜的な集約レイヤーです。各アルゴリズムの sorting/<algo>/cpp/ フォルダは
# それぞれ独立した CMakeLists.txt を持っており、単独で取り出してもビルド可能です。
cmake_minimum_required(VERSION 3.10)
project(SortingAlgorithms CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(sorting/bubble_sort/cpp bubble_sort)
add_subdirectory(sorting/selection_sort/cpp selection_sort)
add_subdirectory(sorting/insertion_sort/cpp insertion_sort)
add_subdirectory(sorting/merge_sort/cpp merge_sort)
add_subdirectory(sorting/quick_sort/cpp quick_sort)
add_subdirectory(sorting/heap_sort/cpp heap_sort)
add_subdirectory(sorting/shell_sort/cpp shell_sort)
add_subdirectory(sorting/counting_sort/cpp counting_sort)

# sort_cli: 8アルゴリズムを実際に試せるCLIサンプル。各アルゴリズムの単独ビルド
# （sorting/<algo>/cpp/CMakeLists.txt）には含めず、このroot集約ビルドにのみ追加する。
add_executable(sort_cli cli/sort_cli.cpp)
target_link_libraries(sort_cli PRIVATE
    bubble_sort
    selection_sort
    insertion_sort
    merge_sort
    quick_sort
    heap_sort
    shell_sort
    counting_sort
)
