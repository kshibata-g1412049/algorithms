"""動的配列 — 倍増戦略による自動拡張

操作:
  append(v)      : 末尾に追加  O(1) 均し
  get(i)         : i番目の要素を返す  O(1)
  set(i, v)      : i番目の要素を設定  O(1)
  insert(i, v)   : i番目の位置に挿入  O(n)
  remove(i)      : i番目の要素を削除  O(n)
  size()         : 要素数  O(1)
  capacity()     : 確保済み容量  O(1)

インデックス範囲外アクセスは IndexError を送出する。
"""


from typing import Generic, TypeVar

T = TypeVar("T")


class DynamicArray(Generic[T]):
    def __init__(self) -> None:
        self._buf = [None] * 4
        self._size = 0

    def _grow(self) -> None:
        new_buf = [None] * (len(self._buf) * 2)
        for i in range(self._size):
            new_buf[i] = self._buf[i]
        self._buf = new_buf

    def _check(self, index: int) -> None:
        if index < 0 or index >= self._size:
            raise IndexError("index out of range")

    def append(self, value: T) -> None:
        if self._size == len(self._buf):
            self._grow()
        self._buf[self._size] = value
        self._size += 1

    def get(self, index: int) -> T:
        self._check(index)
        return self._buf[index]

    def set(self, index: int, value: T) -> None:
        self._check(index)
        self._buf[index] = value

    def insert(self, index: int, value: T) -> None:
        if index < 0 or index > self._size:
            raise IndexError("index out of range")
        if self._size == len(self._buf):
            self._grow()
        for i in range(self._size, index, -1):
            self._buf[i] = self._buf[i - 1]
        self._buf[index] = value
        self._size += 1

    def remove(self, index: int) -> None:
        self._check(index)
        for i in range(index, self._size - 1):
            self._buf[i] = self._buf[i + 1]
        self._buf[self._size - 1] = None
        self._size -= 1

    def size(self) -> int:
        return self._size

    def capacity(self) -> int:
        return len(self._buf)
