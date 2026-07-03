"""隣接リストグラフ（有向グラフ）

操作:
  add_vertex(v)       : O(1)
  add_edge(u, v)      : O(1)
  has_edge(u, v)      : O(deg(u))
  get_neighbors(v)    : O(1)
  vertex_count()      : O(1)
  edge_count()        : O(1)
"""

from typing import Dict, List


class AdjacencyListGraph:
    def __init__(self) -> None:
        self._adj: Dict[int, List[int]] = {}
        self._edge_count = 0

    def add_vertex(self, v: int) -> None:
        if v not in self._adj:
            self._adj[v] = []

    def add_edge(self, u: int, v: int) -> None:
        self.add_vertex(u)
        self.add_vertex(v)
        self._adj[u].append(v)
        self._edge_count += 1

    def has_edge(self, u: int, v: int) -> bool:
        return u in self._adj and v in self._adj[u]

    def get_neighbors(self, v: int) -> List[int]:
        if v not in self._adj:
            raise KeyError(f"vertex {v} not found")
        return self._adj[v]

    def has_vertex(self, v: int) -> bool:
        return v in self._adj

    def vertex_count(self) -> int:
        return len(self._adj)

    def edge_count(self) -> int:
        return self._edge_count
