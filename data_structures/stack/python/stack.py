"""スタック（LIFO）— リストバック実装

操作:
  push(v)  : 要素を積む  O(1) 均し
  pop()    : 先頭を取り出す  O(1)
  peek()   : 先頭を参照（取り出さない）  O(1)
  is_empty(): 空かどうか  O(1)
  size()   : 要素数  O(1)

空状態での pop/peek は IndexError を送出する。
"""


class Stack:
    def __init__(self) -> None:
        self._data: list = []

    def push(self, value: int) -> None:
        self._data.append(value)

    def pop(self) -> int:
        if not self._data:
            raise IndexError("stack is empty")
        return self._data.pop()

    def peek(self) -> int:
        if not self._data:
            raise IndexError("stack is empty")
        return self._data[-1]

    def is_empty(self) -> bool:
        return len(self._data) == 0

    def size(self) -> int:
        return len(self._data)
