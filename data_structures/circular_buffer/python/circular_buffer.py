"""循環バッファ（リングバッファ）— 固定容量

操作:
  push(v)    : 末尾に追加  O(1)  ※満杯なら OverflowError
  pop()      : 先頭を取り出す  O(1)  ※空なら IndexError
  peek()     : 先頭を参照  O(1)
  is_full() / is_empty() / size() / capacity() : O(1)
"""


class CircularBuffer:
    def __init__(self, capacity: int) -> None:
        if capacity <= 0:
            raise ValueError("capacity must be positive")
        self._buf = [0] * capacity
        self._head = 0
        self._count = 0
        self._cap = capacity

    def push(self, value: int) -> None:
        if self._count == self._cap:
            raise OverflowError("circular buffer is full")
        tail = (self._head + self._count) % self._cap
        self._buf[tail] = value
        self._count += 1

    def pop(self) -> int:
        if self._count == 0:
            raise IndexError("circular buffer is empty")
        v = self._buf[self._head]
        self._head = (self._head + 1) % self._cap
        self._count -= 1
        return v

    def peek(self) -> int:
        if self._count == 0:
            raise IndexError("circular buffer is empty")
        return self._buf[self._head]

    def is_empty(self) -> bool:
        return self._count == 0

    def is_full(self) -> bool:
        return self._count == self._cap

    def size(self) -> int:
        return self._count

    def capacity(self) -> int:
        return self._cap
