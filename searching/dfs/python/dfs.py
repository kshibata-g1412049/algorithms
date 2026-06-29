"""深さ優先探索（Depth-First Search, DFS）の実装。

グラフを開始ノードから「行き止まりになるまで深く」探索するアルゴリズム。
あるノードを訪問したら、まだ訪問していない最初の隣接ノード（隣接リストの
並び順で最初に見つかった未訪問ノード）へ進み、それを再帰的に繰り返す。
これ以上進めなくなったら直前のノードに戻り（バックトラック）、
他に未訪問の隣接ノードがあればそちらへ進む。

訪問済みノードを管理する集合を使うことで、サイクルがあっても
同じノードを2回訪問せず、無限ループにならない。

グラフは0始まりの隣接リスト（adjacency[i]がノードiの隣接ノード番号の
リスト）で表現する。

時間複雑度: O(V + E)（Vはノード数、Eは辺数）
空間複雑度: O(V)（訪問済み集合と再帰呼び出しスタックのため）
"""
from typing import List


def dfs(adjacency: List[List[int]], start: int) -> List[int]:
    """startから到達可能なノードの訪問順序（深さ優先）を返す。

    startから到達できないノードは結果に含まれない。
    """
    n = len(adjacency)
    if start < 0 or start >= n:
        return []

    visited = [False] * n
    order: List[int] = []

    def visit(current: int) -> None:
        visited[current] = True
        order.append(current)

        # 隣接リストの並び順で最初に見つかった未訪問ノードへ進む
        for neighbor in adjacency[current]:
            if not visited[neighbor]:
                visit(neighbor)

    visit(start)
    return order
