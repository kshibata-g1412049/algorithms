"""幅優先探索（Breadth-First Search, BFS）の実装。

グラフを開始ノードから「近いノードから順に」探索するアルゴリズム。
キュー（FIFO）を使い、開始ノードをキューに入れた後、キューの先頭から
ノードを取り出して隣接ノードを調べ、未訪問の隣接ノードを隣接リストの
並び順でキューに追加していく。これを繰り返すことで、開始ノードからの
距離が近い順にノードを訪問する。

訪問済みノードを管理する集合を使うことで、サイクルがあっても
同じノードを2回訪問せず、無限ループにならない。

グラフは0始まりの隣接リスト（adjacency[i]がノードiの隣接ノード番号の
リスト）で表現する。

時間複雑度: O(V + E)（Vはノード数、Eは辺数）
空間複雑度: O(V)（訪問済み集合とキューのため）
"""
from collections import deque
from typing import List


def bfs(adjacency: List[List[int]], start: int) -> List[int]:
    """startから到達可能なノードの訪問順序（幅優先）を返す。

    startから到達できないノードは結果に含まれない。
    """
    n = len(adjacency)
    if start < 0 or start >= n:
        return []

    visited = [False] * n
    order = []
    queue = deque([start])
    visited[start] = True

    while queue:
        current = queue.popleft()
        order.append(current)

        # 隣接リストの並び順で未訪問の隣接ノードをキューに追加する
        for neighbor in adjacency[current]:
            if not visited[neighbor]:
                visited[neighbor] = True
                queue.append(neighbor)

    return order
