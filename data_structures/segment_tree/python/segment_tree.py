"""セグメント木（区間和）

操作:
  build(data)      : O(n)
  update(i, value) : 0始まりインデックス  O(log n)
  query(l, r)      : [l, r) の区間和  O(log n)
  size()           : 要素数  O(1)

範囲外アクセスは IndexError。
"""

from typing import List


class SegmentTree:
    def __init__(self, data: List[int]) -> None:
        self._n = len(data)
        self._tree = [0] * (4 * self._n)
        if self._n > 0:
            self._build(data, 1, 0, self._n - 1)

    def _build(self, data: List[int], node: int, lo: int, hi: int) -> None:
        if lo == hi:
            self._tree[node] = data[lo]
            return
        mid = (lo + hi) // 2
        self._build(data, 2 * node, lo, mid)
        self._build(data, 2 * node + 1, mid + 1, hi)
        self._tree[node] = self._tree[2 * node] + self._tree[2 * node + 1]

    def _update(self, node: int, lo: int, hi: int, i: int, value: int) -> None:
        if lo == hi:
            self._tree[node] = value
            return
        mid = (lo + hi) // 2
        if i <= mid:
            self._update(2 * node, lo, mid, i, value)
        else:
            self._update(2 * node + 1, mid + 1, hi, i, value)
        self._tree[node] = self._tree[2 * node] + self._tree[2 * node + 1]

    def update(self, i: int, value: int) -> None:
        if i < 0 or i >= self._n:
            raise IndexError("index out of range")
        self._update(1, 0, self._n - 1, i, value)

    def _query(self, node: int, lo: int, hi: int, l: int, r: int) -> int:
        if r < lo or hi < l:
            return 0
        if l <= lo and hi <= r:
            return self._tree[node]
        mid = (lo + hi) // 2
        return (self._query(2 * node, lo, mid, l, r) +
                self._query(2 * node + 1, mid + 1, hi, l, r))

    def query(self, l: int, r: int) -> int:
        if l < 0 or r > self._n or l >= r:
            raise IndexError("invalid range")
        return self._query(1, 0, self._n - 1, l, r - 1)

    def size(self) -> int:
        return self._n
