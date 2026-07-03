"""ハッシュマップ (int→int、チェイン法)

操作:
  insert(key, value) : O(1)平均
  get(key)           : O(1)平均  — キーがなければ KeyError
  remove(key)        : O(1)平均
  contains(key)      : O(1)平均
  size()             : O(1)

ロードファクタ 0.75 超でリハッシュ（容量2倍）。
"""

from typing import List, Optional, Tuple


class HashMap:
    _LOAD_FACTOR = 0.75

    def __init__(self) -> None:
        self._capacity = 16
        self._buckets: List[List[Tuple[int, int]]] = [[] for _ in range(self._capacity)]
        self._size = 0

    def _idx(self, key: int) -> int:
        return (key * 2654435761) % self._capacity

    def _rehash(self) -> None:
        old = self._buckets
        self._capacity *= 2
        self._buckets = [[] for _ in range(self._capacity)]
        self._size = 0
        for chain in old:
            for k, v in chain:
                self.insert(k, v)

    def insert(self, key: int, value: int) -> None:
        idx = self._idx(key)
        for i, (k, _) in enumerate(self._buckets[idx]):
            if k == key:
                self._buckets[idx][i] = (key, value)
                return
        self._buckets[idx].append((key, value))
        self._size += 1
        if self._size / self._capacity > self._LOAD_FACTOR:
            self._rehash()

    def get(self, key: int) -> int:
        idx = self._idx(key)
        for k, v in self._buckets[idx]:
            if k == key:
                return v
        raise KeyError(key)

    def remove(self, key: int) -> bool:
        idx = self._idx(key)
        for i, (k, _) in enumerate(self._buckets[idx]):
            if k == key:
                self._buckets[idx].pop(i)
                self._size -= 1
                return True
        return False

    def contains(self, key: int) -> bool:
        idx = self._idx(key)
        return any(k == key for k, _ in self._buckets[idx])

    def size(self) -> int:
        return self._size

    def is_empty(self) -> bool:
        return self._size == 0
