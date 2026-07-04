"""キュー（FIFO）— deque バック実装

操作:
  enqueue(v) : 末尾に追加  O(1)
  dequeue()  : 先頭を取り出す  O(1)
  peek()     : 先頭を参照  O(1)
  is_empty() : 空かどうか  O(1)
  size()     : 要素数  O(1)

空状態での dequeue/peek は IndexError を送出する。
"""

from collections import deque


from typing import Generic, TypeVar

T = TypeVar("T")


class Queue(Generic[T]):
    def __init__(self) -> None:
        self._data: deque = deque()

    def enqueue(self, value: T) -> None:
        self._data.append(value)

    def dequeue(self) -> T:
        if not self._data:
            raise IndexError("queue is empty")
        return self._data.popleft()

    def peek(self) -> T:
        if not self._data:
            raise IndexError("queue is empty")
        return self._data[0]

    def is_empty(self) -> bool:
        return len(self._data) == 0

    def size(self) -> int:
        return len(self._data)
