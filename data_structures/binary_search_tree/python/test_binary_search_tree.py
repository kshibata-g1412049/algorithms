import pytest
from binary_search_tree import BinarySearchTree


def test_new_is_empty():
    t = BinarySearchTree()
    assert t.is_empty()
    assert t.size() == 0


def test_insert_and_search():
    t = BinarySearchTree()
    for v in [5, 3, 7, 1, 4]:
        t.insert(v)
    assert t.search(3)
    assert t.search(7)
    assert not t.search(6)
    assert t.size() == 5


def test_inorder_sorted():
    t = BinarySearchTree()
    for v in [5, 3, 7, 1, 4]:
        t.insert(v)
    assert t.inorder() == [1, 3, 4, 5, 7]


def test_preorder():
    t = BinarySearchTree()
    for v in [5, 3, 7]:
        t.insert(v)
    assert t.preorder() == [5, 3, 7]


def test_postorder():
    t = BinarySearchTree()
    for v in [5, 3, 7]:
        t.insert(v)
    assert t.postorder() == [3, 7, 5]


def test_min_max():
    t = BinarySearchTree()
    for v in [5, 3, 7]:
        t.insert(v)
    assert t.min() == 3
    assert t.max() == 7


def test_remove():
    t = BinarySearchTree()
    for v in [5, 3, 7]:
        t.insert(v)
    assert t.remove(3)
    assert t.inorder() == [5, 7]
    assert t.remove(5)
    assert t.inorder() == [7]
    assert not t.remove(99)


def test_duplicate_ignored():
    t = BinarySearchTree()
    t.insert(5)
    t.insert(5)
    assert t.size() == 1


def test_min_empty_raises():
    with pytest.raises(IndexError):
        BinarySearchTree().min()
