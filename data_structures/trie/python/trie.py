"""トライ木（前置木）

操作:
  insert(word)        : 文字列を挿入  O(L)
  search(word)        : 文字列を検索  O(L)
  starts_with(prefix) : プレフィックス検索  O(L)
  remove(word)        : 文字列を削除  O(L)
  size()              : 挿入済み単語数  O(1)

小文字英字 a-z のみ対応。
"""

from typing import Dict, Optional


class _Node:
    __slots__ = ("children", "is_end", "count")

    def __init__(self) -> None:
        self.children: Dict[str, "_Node"] = {}
        self.is_end = False
        self.count = 0


class Trie:
    def __init__(self) -> None:
        self._root = _Node()
        self._size = 0

    def insert(self, word: str) -> None:
        cur = self._root
        for ch in word:
            if ch not in cur.children:
                cur.children[ch] = _Node()
            cur = cur.children[ch]
            cur.count += 1
        if not cur.is_end:
            cur.is_end = True
            self._size += 1

    def search(self, word: str) -> bool:
        cur = self._root
        for ch in word:
            if ch not in cur.children:
                return False
            cur = cur.children[ch]
        return cur.is_end

    def starts_with(self, prefix: str) -> bool:
        cur = self._root
        for ch in prefix:
            if ch not in cur.children:
                return False
            cur = cur.children[ch]
        return True

    def remove(self, word: str) -> bool:
        removed = [False]

        def _remove(node: Optional[_Node], depth: int) -> bool:
            if node is None:
                return False
            if depth == len(word):
                if not node.is_end:
                    return False
                node.is_end = False
                removed[0] = True
                return node.count == 0
            ch = word[depth]
            if ch not in node.children:
                return False
            should_delete = _remove(node.children[ch], depth + 1)
            if removed[0]:
                node.children[ch].count -= 1
            if should_delete:
                del node.children[ch]
            return not node.is_end and node.count == 0

        _remove(self._root, 0)
        if removed[0]:
            self._size -= 1
        return removed[0]

    def size(self) -> int:
        return self._size
