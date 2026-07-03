"""素集合データ構造 (Union-Find)

操作:
  find(x)         : 代表元を返す（経路圧縮）  O(α(n))
  unite(x, y)     : x と y の集合を合併（ランク結合）  O(α(n))
  connected(x, y) : 同じ集合かどうか  O(α(n))
  size()          : 要素数  O(1)
  components()    : 連結成分数  O(1)

頂点ID: 0〜n-1。
"""

from typing import List


class UnionFind:
    def __init__(self, n: int) -> None:
        self._parent: List[int] = list(range(n))
        self._rank: List[int] = [0] * n
        self._n = n
        self._components = n

    def _check(self, x: int) -> None:
        if x < 0 or x >= self._n:
            raise IndexError(f"element {x} out of range")

    def find(self, x: int) -> int:
        self._check(x)
        if self._parent[x] != x:
            self._parent[x] = self.find(self._parent[x])
        return self._parent[x]

    def unite(self, x: int, y: int) -> bool:
        rx, ry = self.find(x), self.find(y)
        if rx == ry:
            return False
        if self._rank[rx] < self._rank[ry]:
            rx, ry = ry, rx
        self._parent[ry] = rx
        if self._rank[rx] == self._rank[ry]:
            self._rank[rx] += 1
        self._components -= 1
        return True

    def connected(self, x: int, y: int) -> bool:
        return self.find(x) == self.find(y)

    def size(self) -> int:
        return self._n

    def components(self) -> int:
        return self._components
