"""bfs関数のテスト（pytest）。

このファイルは同じディレクトリ内のbfs.pyのみに依存しているため、
python/フォルダを単独で取り出してもこのテストは実行できる。
"""
from bfs import bfs


def test_single_node():
    """単一ノードのグラフ（隣接リストが空のノード1つ）"""
    adjacency = [[]]
    assert bfs(adjacency, 0) == [0]


def test_linear_graph():
    """線形グラフ（0-1-2-3が一直線につながっている）"""
    adjacency = [
        [1],      # 0
        [0, 2],   # 1
        [1, 3],   # 2
        [2],      # 3
    ]
    assert bfs(adjacency, 0) == [0, 1, 2, 3]


def test_branching_tree():
    """分岐のある木構造のグラフ
    0 -- 1 -- 3
    |    |
    2    4
    """
    adjacency = [
        [1, 2],      # 0
        [0, 3, 4],   # 1
        [0],         # 2
        [1],         # 3
        [1],         # 4
    ]
    assert bfs(adjacency, 0) == [0, 1, 2, 3, 4]


def test_graph_with_cycle():
    """サイクルを含むグラフ（0-1-2-3-0のループ）。無限ループしないことを確認する。"""
    adjacency = [
        [1],       # 0
        [2],       # 1
        [0, 3],    # 2
        [2],       # 3
    ]
    assert bfs(adjacency, 0) == [0, 1, 2, 3]


def test_disconnected_graph_with_isolated_node():
    """孤立ノードを含む非連結グラフ。startから到達できないノードは結果に含まれない。"""
    adjacency = [
        [1],   # 0
        [0],   # 1
        [],    # 2 (孤立ノード、0からは到達不可)
    ]
    assert bfs(adjacency, 0) == [0, 1]


def test_multiple_branches():
    """BFSとDFSで訪問順序が異なることを確認するための、複数の子を持つグラフ
    0 -- 1 -- 4
    |
    2
    |
    3
    """
    adjacency = [
        [1, 2, 3],   # 0
        [0, 4],      # 1
        [0],         # 2
        [0],         # 3
        [1],         # 4
    ]
    assert bfs(adjacency, 0) == [0, 1, 2, 3, 4]
