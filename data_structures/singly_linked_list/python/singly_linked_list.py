"""単方向連結リスト

操作:
  push_front(v)       : 先頭に挿入  O(1)
  push_back(v)        : 末尾に追加  O(n)
  pop_front()         : 先頭を取り出す  O(1)
  front() / back()    : 先頭/末尾の参照  O(1)/O(n)
  contains(v)         : 値を検索  O(n)
  remove(v)           : 最初に見つかった値を削除  O(n)
  size() / is_empty() : O(1)
  to_list()           : 先頭→末尾のコピーリスト  O(n)
"""

from typing import List, Optional


from typing import Generic, TypeVar

T = TypeVar("T")


class _Node:
    __slots__ = ("val", "next")

    def __init__(self, val: T) -> None:
        self.val = val
        self.next: Optional["_Node"] = None


class SinglyLinkedList(Generic[T]):
    def __init__(self) -> None:
        self._head: Optional[_Node] = None
        self._size = 0

    def push_front(self, value: T) -> None:
        node = _Node(value)
        node.next = self._head
        self._head = node
        self._size += 1

    def push_back(self, value: T) -> None:
        node = _Node(value)
        if self._head is None:
            self._head = node
        else:
            cur = self._head
            while cur.next:
                cur = cur.next
            cur.next = node
        self._size += 1

    def pop_front(self) -> T:
        if self._head is None:
            raise IndexError("list is empty")
        v = self._head.val
        self._head = self._head.next
        self._size -= 1
        return v

    def front(self) -> T:
        if self._head is None:
            raise IndexError("list is empty")
        return self._head.val

    def back(self) -> T:
        if self._head is None:
            raise IndexError("list is empty")
        cur = self._head
        while cur.next:
            cur = cur.next
        return cur.val

    def contains(self, value: T) -> bool:
        cur = self._head
        while cur:
            if cur.val == value:
                return True
            cur = cur.next
        return False

    def remove(self, value: T) -> bool:
        if self._head is None:
            return False
        if self._head.val == value:
            self._head = self._head.next
            self._size -= 1
            return True
        cur = self._head
        while cur.next:
            if cur.next.val == value:
                cur.next = cur.next.next
                self._size -= 1
                return True
            cur = cur.next
        return False

    def size(self) -> int:
        return self._size

    def is_empty(self) -> bool:
        return self._size == 0

    def to_list(self) -> List[T]:
        result = []
        cur = self._head
        while cur:
            result.append(cur.val)
            cur = cur.next
        return result
