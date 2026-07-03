"""隣接行列グラフ（有向グラフ、固定頂点数）

操作:
  add_edge(u, v)       : O(1)
  remove_edge(u, v)    : O(1)
  has_edge(u, v)       : O(1)
  get_neighbors(v)     : O(V)
  vertex_count()       : O(1)
  edge_count()         : O(1)

コンストラクタで頂点数を指定（頂点ID: 0〜n-1）。
"""

from typing import List


class AdjacencyMatrixGraph:
    def __init__(self, n: int) -> None:
        self._n = n
        self._matrix = [[False] * n for _ in range(n)]
        self._edge_count = 0

    def _check(self, v: int) -> None:
        if v < 0 or v >= self._n:
            raise IndexError(f"vertex {v} out of range")

    def add_edge(self, u: int, v: int) -> None:
        self._check(u)
        self._check(v)
        if not self._matrix[u][v]:
            self._matrix[u][v] = True
            self._edge_count += 1

    def remove_edge(self, u: int, v: int) -> None:
        self._check(u)
        self._check(v)
        if self._matrix[u][v]:
            self._matrix[u][v] = False
            self._edge_count -= 1

    def has_edge(self, u: int, v: int) -> bool:
        self._check(u)
        self._check(v)
        return self._matrix[u][v]

    def get_neighbors(self, v: int) -> List[int]:
        self._check(v)
        return [i for i in range(self._n) if self._matrix[v][i]]

    def vertex_count(self) -> int:
        return self._n

    def edge_count(self) -> int:
        return self._edge_count
