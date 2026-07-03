import math
import pytest
from avl_tree import AvlTree


def test_new_is_empty():
    t = AvlTree()
    assert t.is_empty()
    assert t.size() == 0


def test_insert_and_search():
    t = AvlTree()
    for v in [5, 3, 7, 1, 4]:
        t.insert(v)
    assert t.search(3)
    assert not t.search(6)
    assert t.inorder() == [1, 3, 4, 5, 7]


def test_min_max():
    t = AvlTree()
    for v in [5, 3, 7]:
        t.insert(v)
    assert t.min() == 3
    assert t.max() == 7


def test_height_log_n():
    t = AvlTree()
    for i in range(1, 101):
        t.insert(i)
    h = t.height()
    expected = math.ceil(1.44 * math.log2(101))
    assert h <= expected + 1


def test_inorder_sorted():
    t = AvlTree()
    for i in range(1, 101):
        t.insert(i)
    io = t.inorder()
    assert io == list(range(1, 101))


def test_remove():
    t = AvlTree()
    for v in [5, 3, 7, 1, 4]:
        t.insert(v)
    assert t.remove(1)
    assert t.remove(3)
    assert t.inorder() == [4, 5, 7]
    assert not t.remove(99)


def test_min_empty_raises():
    with pytest.raises(IndexError):
        AvlTree().min()
