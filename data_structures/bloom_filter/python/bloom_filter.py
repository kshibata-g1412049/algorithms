"""ブルームフィルタ

操作:
  insert(v)   : O(k)
  contains(v) : O(k) — 偽陽性あり、偽陰性なし

k 個の独立ハッシュ関数で m ビットのビット配列を使用。
"""


from typing import Generic, TypeVar

T = TypeVar("T")


class BloomFilter(Generic[T]):
    def __init__(self, m: int, k: int) -> None:
        self._m = m
        self._k = k
        self._bits = bytearray(m)

    def _hash(self, value: T, seed: int) -> int:
        h = hash(value) ^ (seed * 0x9E3779B9)
        h &= 0xFFFFFFFF
        h ^= h >> 16
        h = (h * 0x85EBCA6B) & 0xFFFFFFFF
        h ^= h >> 13
        h = (h * 0xC2B2AE35) & 0xFFFFFFFF
        h ^= h >> 16
        return h % self._m

    def insert(self, value: T) -> None:
        for i in range(self._k):
            self._bits[self._hash(value, i)] = 1

    def contains(self, value: T) -> bool:
        return all(self._bits[self._hash(value, i)] for i in range(self._k))
