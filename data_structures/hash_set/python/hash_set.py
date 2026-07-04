"""ハッシュセット（線形探索法）

操作:
  insert(v)   : O(1)平均
  contains(v) : O(1)平均
  remove(v)   : O(1)平均
  size()      : O(1)

ロードファクタ 0.5 超でリハッシュ（容量2倍）。
削除は tombstone マーキングを使用。
"""

from enum import Enum, auto
from typing import List, Optional


from typing import Generic, TypeVar

T = TypeVar("T")


class _State(Enum):
    EMPTY = auto()
    USED = auto()
    DELETED = auto()


class HashSet(Generic[T]):
    _LOAD_FACTOR = 0.5

    def __init__(self) -> None:
        self._capacity = 16
        self._table: List[Optional[T]] = [None] * self._capacity
        self._state: List[_State] = [_State.EMPTY] * self._capacity
        self._size = 0

    def _idx(self, value: T) -> int:
        return (hash(value) * 2654435761) % self._capacity

    def _probe(self, value: T) -> int:
        i = self._idx(value)
        while (self._state[i] == _State.USED and self._table[i] != value):
            i = (i + 1) % self._capacity
        return i

    def _rehash(self) -> None:
        old_table = self._table[:]
        old_state = self._state[:]
        self._capacity *= 2
        self._table = [None] * self._capacity
        self._state = [_State.EMPTY] * self._capacity
        self._size = 0
        for v, s in zip(old_table, old_state):
            if s == _State.USED:
                self.insert(v)

    def insert(self, value: T) -> None:
        if (self._size + 1) / self._capacity > self._LOAD_FACTOR:
            self._rehash()
        i = self._probe(value)
        if self._state[i] != _State.USED:
            self._table[i] = value
            self._state[i] = _State.USED
            self._size += 1

    def contains(self, value: T) -> bool:
        i = self._probe(value)
        return self._state[i] == _State.USED and self._table[i] == value

    def remove(self, value: T) -> bool:
        i = self._probe(value)
        if self._state[i] != _State.USED or self._table[i] != value:
            return False
        self._state[i] = _State.DELETED
        self._size -= 1
        return True

    def size(self) -> int:
        return self._size

    def is_empty(self) -> bool:
        return self._size == 0
