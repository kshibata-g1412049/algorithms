"""二分探索木 (BST)

操作:
  insert(v)                 : 挿入  O(h)
  search(v)                 : 検索  O(h)
  remove(v)                 : 削除  O(h)
  min() / max()             : 最小/最大値  O(h)
  inorder/preorder/postorder: 走査  O(n)
  size() / is_empty()       : O(1)

重複値は無視。空木での min/max は IndexError。
"""

from typing import List, Optional


class _Node:
    __slots__ = ("val", "left", "right")

    def __init__(self, val: int) -> None:
        self.val = val
        self.left: Optional["_Node"] = None
        self.right: Optional["_Node"] = None


class BinarySearchTree:
    def __init__(self) -> None:
        self._root: Optional[_Node] = None
        self._size = 0

    def insert(self, value: int) -> None:
        def _insert(node: Optional[_Node]) -> _Node:
            if node is None:
                self._size += 1
                return _Node(value)
            if value < node.val:
                node.left = _insert(node.left)
            elif value > node.val:
                node.right = _insert(node.right)
            return node
        self._root = _insert(self._root)

    def search(self, value: int) -> bool:
        node = self._root
        while node:
            if value == node.val:
                return True
            node = node.left if value < node.val else node.right
        return False

    def remove(self, value: int) -> bool:
        found = [False]

        def _remove(node: Optional[_Node]) -> Optional[_Node]:
            if node is None:
                return None
            if value < node.val:
                node.left = _remove(node.left)
            elif value > node.val:
                node.right = _remove(node.right)
            else:
                found[0] = True
                if node.left is None:
                    return node.right
                if node.right is None:
                    return node.left
                succ = node.right
                while succ.left:
                    succ = succ.left
                node.val = succ.val
                node.right = _remove(node.right)
                found[0] = True
            return node

        self._root = _remove(self._root)
        if found[0]:
            self._size -= 1
        return found[0]

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

    def preorder(self) -> List[int]:
        result: List[int] = []

        def _visit(node: Optional[_Node]) -> None:
            if node is None:
                return
            result.append(node.val)
            _visit(node.left)
            _visit(node.right)

        _visit(self._root)
        return result

    def postorder(self) -> List[int]:
        result: List[int] = []

        def _visit(node: Optional[_Node]) -> None:
            if node is None:
                return
            _visit(node.left)
            _visit(node.right)
            result.append(node.val)

        _visit(self._root)
        return result
