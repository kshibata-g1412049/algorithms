"""二分ヒープ（最小ヒープ）

操作:
  insert(v)           : O(log n)
  find_min()          : O(1)
  extract_min()       : O(log n)
  size() / is_empty() : O(1)

空ヒープでの find_min/extract_min は IndexError。
"""

from typing import List


from typing import Generic, TypeVar

T = TypeVar("T")


class BinaryHeap(Generic[T]):
    def __init__(self) -> None:
        self._data: List[T] = []

    def _sift_up(self, i: int) -> None:
        while i > 0:
            p = (i - 1) // 2
            if self._data[p] <= self._data[i]:
                break
            self._data[p], self._data[i] = self._data[i], self._data[p]
            i = p

    def _sift_down(self, i: int) -> None:
        n = len(self._data)
        while True:
            smallest = i
            l, r = 2 * i + 1, 2 * i + 2
            if l < n and self._data[l] < self._data[smallest]:
                smallest = l
            if r < n and self._data[r] < self._data[smallest]:
                smallest = r
            if smallest == i:
                break
            self._data[i], self._data[smallest] = self._data[smallest], self._data[i]
            i = smallest

    def insert(self, value: T) -> None:
        self._data.append(value)
        self._sift_up(len(self._data) - 1)

    def find_min(self) -> T:
        if not self._data:
            raise IndexError("heap is empty")
        return self._data[0]

    def extract_min(self) -> T:
        if not self._data:
            raise IndexError("heap is empty")
        v = self._data[0]
        self._data[0] = self._data[-1]
        self._data.pop()
        if self._data:
            self._sift_down(0)
        return v

    def size(self) -> int:
        return len(self._data)

    def is_empty(self) -> bool:
        return len(self._data) == 0
