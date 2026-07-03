"""フェニック木 (Binary Indexed Tree)

操作:
  update(i, delta)  : i番目（0始まり）に delta を加算  O(log n)
  prefix_sum(i)     : [0, i) の前置和  O(log n)
  range_sum(l, r)   : [l, r) の区間和  O(log n)
  size()            : 要素数  O(1)

範囲外アクセスは IndexError。
"""

from typing import List


class FenwickTree:
    def __init__(self, n_or_data) -> None:
        if isinstance(n_or_data, int):
            self._n = n_or_data
            self._tree = [0] * (self._n + 1)
        else:
            data: List[int] = n_or_data
            self._n = len(data)
            self._tree = [0] * (self._n + 1)
            for i, v in enumerate(data):
                self.update(i, v)

    def update(self, i: int, delta: int) -> None:
        if i < 0 or i >= self._n:
            raise IndexError("index out of range")
        i += 1
        while i <= self._n:
            self._tree[i] += delta
            i += i & (-i)

    def prefix_sum(self, i: int) -> int:
        if i < 0 or i > self._n:
            raise IndexError("index out of range")
        s = 0
        while i > 0:
            s += self._tree[i]
            i -= i & (-i)
        return s

    def range_sum(self, l: int, r: int) -> int:
        if l < 0 or r > self._n or l > r:
            raise IndexError("invalid range")
        return self.prefix_sum(r) - self.prefix_sum(l)

    def size(self) -> int:
        return self._n
