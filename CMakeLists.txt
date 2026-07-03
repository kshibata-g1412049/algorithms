# ルートCMakeLists.txt
#
# このファイルはリポジトリ内の全アルゴリズムのC++実装をまとめてビルド・テストするための
# 便宜的な集約レイヤーです。各アルゴリズムの sorting/<algo>/cpp/ や searching/<algo>/cpp/
# フォルダはそれぞれ独立した CMakeLists.txt を持っており、単独で取り出してもビルド可能です。
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

# 配列・リスト探索（7種）
add_subdirectory(searching/linear_search/cpp linear_search)
add_subdirectory(searching/binary_search/cpp binary_search)
add_subdirectory(searching/jump_search/cpp jump_search)
add_subdirectory(searching/interpolation_search/cpp interpolation_search)
add_subdirectory(searching/exponential_search/cpp exponential_search)
add_subdirectory(searching/ternary_search/cpp ternary_search)
add_subdirectory(searching/fibonacci_search/cpp fibonacci_search)

# 基本グラフ探索（2種）
add_subdirectory(searching/bfs/cpp bfs)
add_subdirectory(searching/dfs/cpp dfs)

# A*系（6種）
add_subdirectory(searching/dijkstra/cpp dijkstra)
add_subdirectory(searching/astar/cpp astar)
add_subdirectory(searching/weighted_astar/cpp weighted_astar)
add_subdirectory(searching/bidirectional_astar/cpp bidirectional_astar)
add_subdirectory(searching/jump_point_search/cpp jump_point_search)
add_subdirectory(searching/ida_star/cpp ida_star)

# RRT系（5種）
add_subdirectory(searching/rrt/cpp rrt)
add_subdirectory(searching/goal_biased_rrt/cpp goal_biased_rrt)
add_subdirectory(searching/rrt_star/cpp rrt_star)
add_subdirectory(searching/rrt_connect/cpp rrt_connect)
add_subdirectory(searching/informed_rrt_star/cpp informed_rrt_star)

# search_cli: 配列探索7種を試せるCLIサンプル（sort_cliと同じ単純な位置引数方式）。
add_executable(search_cli cli/search_cli.cpp)
target_link_libraries(search_cli PRIVATE
    linear_search
    binary_search
    jump_search
    interpolation_search
    exponential_search
    ternary_search
    fibonacci_search
)

# graph_cli/grid_cli/rrt_cli: グラフ・グリッド・連続空間+障害物といった複雑な
# 入力はCLI引数では表現しにくいため、YAML設定ファイルのパスを引数で受け取って
# 読み込む方式にしている。YAML解析にはyaml-cppを使用する（CLI層だけの依存とし、
# searching/*/cpp の各アルゴリズム本体には持ち込まない）。
find_package(yaml-cpp REQUIRED)

add_executable(graph_cli cli/graph_cli.cpp cli/graph_cli_config.cpp)
target_link_libraries(graph_cli PRIVATE bfs dfs yaml-cpp::yaml-cpp)

add_executable(grid_cli cli/grid_cli.cpp cli/grid_cli_config.cpp)
target_link_libraries(grid_cli PRIVATE
    dijkstra
    astar
    weighted_astar
    bidirectional_astar
    jump_point_search
    ida_star
    yaml-cpp::yaml-cpp
)

add_executable(rrt_cli cli/rrt_cli.cpp cli/rrt_cli_config.cpp)
target_link_libraries(rrt_cli PRIVATE
    rrt
    goal_biased_rrt
    rrt_star
    rrt_connect
    informed_rrt_star
    yaml-cpp::yaml-cpp
)

# CLI設定読み込みヘルパーの単体テスト（正常系・各種異常系を網羅）。
add_executable(test_graph_cli_config cli/test_graph_cli_config.cpp cli/graph_cli_config.cpp)
target_include_directories(test_graph_cli_config PRIVATE cli)
target_link_libraries(test_graph_cli_config PRIVATE yaml-cpp::yaml-cpp)
add_test(NAME graph_cli_config_test COMMAND test_graph_cli_config)

add_executable(test_grid_cli_config cli/test_grid_cli_config.cpp cli/grid_cli_config.cpp)
target_include_directories(test_grid_cli_config PRIVATE cli)
target_link_libraries(test_grid_cli_config PRIVATE yaml-cpp::yaml-cpp)
add_test(NAME grid_cli_config_test COMMAND test_grid_cli_config)

add_executable(test_rrt_cli_config cli/test_rrt_cli_config.cpp cli/rrt_cli_config.cpp)
target_include_directories(test_rrt_cli_config PRIVATE cli)
target_link_libraries(test_rrt_cli_config PRIVATE yaml-cpp::yaml-cpp)
add_test(NAME rrt_cli_config_test COMMAND test_rrt_cli_config)

# graph_cli/grid_cli/rrt_cliのエンドツーエンドスモークテスト
# （正常系の出力確認、異常系の異常終了確認）。
add_test(NAME graph_cli_smoke_bfs
         COMMAND graph_cli bfs ${CMAKE_CURRENT_SOURCE_DIR}/cli/configs/graph_example.yaml)
set_tests_properties(graph_cli_smoke_bfs PROPERTIES PASS_REGULAR_EXPRESSION "0 1 2 3")

add_test(NAME graph_cli_smoke_missing_file
         COMMAND graph_cli bfs ${CMAKE_CURRENT_SOURCE_DIR}/cli/configs/does_not_exist.yaml)
set_tests_properties(graph_cli_smoke_missing_file PROPERTIES WILL_FAIL TRUE)

add_test(NAME grid_cli_smoke_astar
         COMMAND grid_cli astar ${CMAKE_CURRENT_SOURCE_DIR}/cli/configs/grid_example.yaml)
set_tests_properties(grid_cli_smoke_astar PROPERTIES PASS_REGULAR_EXPRESSION "\\(0,0\\).*\\(4,4\\)")

add_test(NAME grid_cli_smoke_missing_file
         COMMAND grid_cli astar ${CMAKE_CURRENT_SOURCE_DIR}/cli/configs/does_not_exist.yaml)
set_tests_properties(grid_cli_smoke_missing_file PROPERTIES WILL_FAIL TRUE)

add_test(NAME rrt_cli_smoke_rrt_star
         COMMAND rrt_cli rrt_star ${CMAKE_CURRENT_SOURCE_DIR}/cli/configs/rrt_example.yaml)
set_tests_properties(rrt_cli_smoke_rrt_star PROPERTIES PASS_REGULAR_EXPRESSION "^\\(1(\\.0+)?,1(\\.0+)?\\)")

add_test(NAME rrt_cli_smoke_missing_file
         COMMAND rrt_cli rrt ${CMAKE_CURRENT_SOURCE_DIR}/cli/configs/does_not_exist.yaml)
set_tests_properties(rrt_cli_smoke_missing_file PROPERTIES WILL_FAIL TRUE)

# ── データ構造（20種） ──────────────────────────────────────────────────────────

# 線形データ構造（7種）
add_subdirectory(data_structures/stack/cpp ds_stack)
add_subdirectory(data_structures/queue/cpp ds_queue)
add_subdirectory(data_structures/deque/cpp ds_deque)
add_subdirectory(data_structures/singly_linked_list/cpp ds_singly_linked_list)
add_subdirectory(data_structures/doubly_linked_list/cpp ds_doubly_linked_list)
add_subdirectory(data_structures/circular_buffer/cpp ds_circular_buffer)
add_subdirectory(data_structures/dynamic_array/cpp ds_dynamic_array)

# 木構造（6種）
add_subdirectory(data_structures/binary_search_tree/cpp ds_binary_search_tree)
add_subdirectory(data_structures/avl_tree/cpp ds_avl_tree)
add_subdirectory(data_structures/binary_heap/cpp ds_binary_heap)
add_subdirectory(data_structures/trie/cpp ds_trie)
add_subdirectory(data_structures/segment_tree/cpp ds_segment_tree)
add_subdirectory(data_structures/fenwick_tree/cpp ds_fenwick_tree)

# ハッシュ系（2種）
add_subdirectory(data_structures/hash_map/cpp ds_hash_map)
add_subdirectory(data_structures/hash_set/cpp ds_hash_set)

# グラフ表現（3種）
add_subdirectory(data_structures/adjacency_list_graph/cpp ds_adjacency_list_graph)
add_subdirectory(data_structures/adjacency_matrix_graph/cpp ds_adjacency_matrix_graph)
add_subdirectory(data_structures/union_find/cpp ds_union_find)

# 応用・確率的（2種）
add_subdirectory(data_structures/bloom_filter/cpp ds_bloom_filter)
add_subdirectory(data_structures/skip_list/cpp ds_skip_list)
