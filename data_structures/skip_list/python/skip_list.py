"""スキップリスト（確率的データ構造）

操作:
  insert(v)   : 平均 O(log n)
  search(v)   : 平均 O(log n)
  remove(v)   : 平均 O(log n)
  size()      : O(1)

最大レベル 16、各レベル昇格確率 0.5。
再現性のためシードを受け取る。
"""

from typing import List, Optional
import random


_MAX_LEVEL = 16


class _Node:
    __slots__ = ("val", "forward")

    def __init__(self, val: int, level: int) -> None:
        self.val = val
        self.forward: List[Optional["_Node"]] = [None] * (level + 1)


class SkipList:
    def __init__(self, seed: int = 42) -> None:
        self._head = _Node(-(1 << 31), _MAX_LEVEL)
        self._level = 0
        self._size = 0
        self._rng = random.Random(seed)

    def _random_level(self) -> int:
        lvl = 0
        while lvl < _MAX_LEVEL and self._rng.random() < 0.5:
            lvl += 1
        return lvl

    def insert(self, value: int) -> None:
        update: List[Optional[_Node]] = [None] * (_MAX_LEVEL + 1)
        cur = self._head
        for i in range(self._level, -1, -1):
            while cur.forward[i] and cur.forward[i].val < value:
                cur = cur.forward[i]
            update[i] = cur
        cur = cur.forward[0]
        if cur and cur.val == value:
            return
        lvl = self._random_level()
        if lvl > self._level:
            for i in range(self._level + 1, lvl + 1):
                update[i] = self._head
            self._level = lvl
        node = _Node(value, lvl)
        for i in range(lvl + 1):
            node.forward[i] = update[i].forward[i]
            update[i].forward[i] = node
        self._size += 1

    def search(self, value: int) -> bool:
        cur = self._head
        for i in range(self._level, -1, -1):
            while cur.forward[i] and cur.forward[i].val < value:
                cur = cur.forward[i]
        cur = cur.forward[0]
        return cur is not None and cur.val == value

    def remove(self, value: int) -> bool:
        update: List[Optional[_Node]] = [None] * (_MAX_LEVEL + 1)
        cur = self._head
        for i in range(self._level, -1, -1):
            while cur.forward[i] and cur.forward[i].val < value:
                cur = cur.forward[i]
            update[i] = cur
        cur = cur.forward[0]
        if not cur or cur.val != value:
            return False
        for i in range(self._level + 1):
            if update[i].forward[i] != cur:
                break
            update[i].forward[i] = cur.forward[i]
        while self._level > 0 and self._head.forward[self._level] is None:
            self._level -= 1
        self._size -= 1
        return True

    def size(self) -> int:
        return self._size

    def is_empty(self) -> bool:
        return self._size == 0
