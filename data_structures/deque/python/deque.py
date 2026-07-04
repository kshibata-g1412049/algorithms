"""両端キュー（Deque）— collections.deque バック実装

操作:
  push_front(v) / push_back(v)  : 両端への追加  O(1)
  pop_front()  / pop_back()     : 両端からの取り出し  O(1)
  peek_front() / peek_back()    : 両端の参照  O(1)
  is_empty() / size()           : O(1)

空状態での pop*/peek* は IndexError を送出する。
"""

from collections import deque as _deque


from typing import Generic, TypeVar

T = TypeVar("T")


class Deque(Generic[T]):
    def __init__(self) -> None:
        self._data: _deque = _deque()

    def push_front(self, value: T) -> None:
        self._data.appendleft(value)

    def push_back(self, value: T) -> None:
        self._data.append(value)

    def pop_front(self) -> T:
        if not self._data:
            raise IndexError("deque is empty")
        return self._data.popleft()

    def pop_back(self) -> T:
        if not self._data:
            raise IndexError("deque is empty")
        return self._data.pop()

    def peek_front(self) -> T:
        if not self._data:
            raise IndexError("deque is empty")
        return self._data[0]

    def peek_back(self) -> T:
        if not self._data:
            raise IndexError("deque is empty")
        return self._data[-1]

    def is_empty(self) -> bool:
        return len(self._data) == 0

    def size(self) -> int:
        return len(self._data)
