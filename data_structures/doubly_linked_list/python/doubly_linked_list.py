"""双方向連結リスト

操作:
  push_front(v) / push_back(v) : O(1)
  pop_front() / pop_back()     : O(1)
  front() / back()             : O(1)
  contains(v) / remove(v)      : O(n)
  size() / is_empty()          : O(1)
  to_list()                    : O(n)
"""

from typing import List, Optional


class _Node:
    __slots__ = ("val", "prev", "next")

    def __init__(self, val: int) -> None:
        self.val = val
        self.prev: Optional["_Node"] = None
        self.next: Optional["_Node"] = None


class DoublyLinkedList:
    def __init__(self) -> None:
        self._head: Optional[_Node] = None
        self._tail: Optional[_Node] = None
        self._size = 0

    def _unlink(self, node: _Node) -> None:
        if node.prev:
            node.prev.next = node.next
        else:
            self._head = node.next
        if node.next:
            node.next.prev = node.prev
        else:
            self._tail = node.prev
        self._size -= 1

    def push_front(self, value: int) -> None:
        node = _Node(value)
        node.next = self._head
        if self._head:
            self._head.prev = node
        else:
            self._tail = node
        self._head = node
        self._size += 1

    def push_back(self, value: int) -> None:
        node = _Node(value)
        node.prev = self._tail
        if self._tail:
            self._tail.next = node
        else:
            self._head = node
        self._tail = node
        self._size += 1

    def pop_front(self) -> int:
        if self._head is None:
            raise IndexError("list is empty")
        v = self._head.val
        self._unlink(self._head)
        return v

    def pop_back(self) -> int:
        if self._tail is None:
            raise IndexError("list is empty")
        v = self._tail.val
        self._unlink(self._tail)
        return v

    def front(self) -> int:
        if self._head is None:
            raise IndexError("list is empty")
        return self._head.val

    def back(self) -> int:
        if self._tail is None:
            raise IndexError("list is empty")
        return self._tail.val

    def contains(self, value: int) -> bool:
        cur = self._head
        while cur:
            if cur.val == value:
                return True
            cur = cur.next
        return False

    def remove(self, value: int) -> bool:
        cur = self._head
        while cur:
            if cur.val == value:
                self._unlink(cur)
                return True
            cur = cur.next
        return False

    def size(self) -> int:
        return self._size

    def is_empty(self) -> bool:
        return self._size == 0

    def to_list(self) -> List[int]:
        result = []
        cur = self._head
        while cur:
            result.append(cur.val)
            cur = cur.next
        return result
