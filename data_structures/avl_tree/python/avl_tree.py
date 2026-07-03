"""AVL木（自己平衡二分探索木）

操作:
  insert(v) / remove(v) / search(v) : O(log n)
  min() / max()                     : O(log n)
  inorder()                         : O(n)
  height() / size() / is_empty()    : O(1)

重複値は無視。空木での min/max は IndexError。
"""

from typing import List, Optional


class _Node:
    __slots__ = ("val", "ht", "left", "right")

    def __init__(self, val: int) -> None:
        self.val = val
        self.ht = 1
        self.left: Optional["_Node"] = None
        self.right: Optional["_Node"] = None


def _ht(n: Optional[_Node]) -> int:
    return n.ht if n else 0


def _update_ht(n: _Node) -> None:
    n.ht = 1 + max(_ht(n.left), _ht(n.right))


def _bf(n: _Node) -> int:
    return _ht(n.left) - _ht(n.right)


def _rot_right(y: _Node) -> _Node:
    x = y.left
    y.left = x.right
    x.right = y
    _update_ht(y)
    _update_ht(x)
    return x


def _rot_left(x: _Node) -> _Node:
    y = x.right
    x.right = y.left
    y.left = x
    _update_ht(x)
    _update_ht(y)
    return y


def _balance(n: _Node) -> _Node:
    _update_ht(n)
    b = _bf(n)
    if b > 1:
        if _bf(n.left) < 0:
            n.left = _rot_left(n.left)
        return _rot_right(n)
    if b < -1:
        if _bf(n.right) > 0:
            n.right = _rot_right(n.right)
        return _rot_left(n)
    return n


class AvlTree:
    def __init__(self) -> None:
        self._root: Optional[_Node] = None
        self._size = 0

    def insert(self, value: int) -> None:
        inserted = [False]

        def _insert(node: Optional[_Node]) -> _Node:
            if node is None:
                inserted[0] = True
                return _Node(value)
            if value < node.val:
                node.left = _insert(node.left)
            elif value > node.val:
                node.right = _insert(node.right)
            return _balance(node)

        self._root = _insert(self._root)
        if inserted[0]:
            self._size += 1

    def remove(self, value: int) -> bool:
        removed = [False]

        def _min_node(node: _Node) -> _Node:
            while node.left:
                node = node.left
            return node

        def _remove(node: Optional[_Node]) -> Optional[_Node]:
            if node is None:
                return None
            if value < node.val:
                node.left = _remove(node.left)
            elif value > node.val:
                node.right = _remove(node.right)
            else:
                removed[0] = True
                if node.left is None:
                    return node.right
                if node.right is None:
                    return node.left
                succ = _min_node(node.right)
                node.val = succ.val
                node.right = _remove(node.right)
                removed[0] = True
            return _balance(node)

        self._root = _remove(self._root)
        if removed[0]:
            self._size -= 1
        return removed[0]

    def search(self, value: int) -> bool:
        node = self._root
        while node:
            if value == node.val:
                return True
            node = node.left if value < node.val else node.right
        return False

    def min(self) -> int:
        if self._root is None:
            raise IndexError("tree is empty")
        node = self._root
        while node.left:
            node = node.left
        return node.val

    def max(self) -> int:
        if self._root is None:
            raise IndexError("tree is empty")
        node = self._root
        while node.right:
            node = node.right
        return node.val

    def height(self) -> int:
        return _ht(self._root)

    def size(self) -> int:
        return self._size

    def is_empty(self) -> bool:
        return self._size == 0

    def inorder(self) -> List[int]:
        result: List[int] = []

        def _visit(node: Optional[_Node]) -> None:
            if node is None:
                return
            _visit(node.left)
            result.append(node.val)
            _visit(node.right)

        _visit(self._root)
        return result
