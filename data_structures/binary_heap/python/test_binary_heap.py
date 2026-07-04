import pytest
from binary_heap import BinaryHeap


def test_new_is_empty():
    h = BinaryHeap()
    assert h.is_empty()
    assert h.size() == 0


def test_find_min():
    h = BinaryHeap()
    for v in [5, 3, 7, 1]:
        h.insert(v)
    assert h.find_min() == 1
    assert h.size() == 4


def test_extract_min_order():
    h = BinaryHeap()
    for v in [5, 3, 7, 1, 4, 2, 6]:
        h.insert(v)
    result = []
    while not h.is_empty():
        result.append(h.extract_min())
    assert result == [1, 2, 3, 4, 5, 6, 7]


def test_100_elements():
    h = BinaryHeap()
    for i in range(100, 0, -1):
        h.insert(i)
    prev = h.extract_min()
    while not h.is_empty():
        cur = h.extract_min()
        assert cur >= prev
        prev = cur


def test_find_min_empty_raises():
    with pytest.raises(IndexError):
        BinaryHeap().find_min()


def test_extract_min_empty_raises():
    with pytest.raises(IndexError):
        BinaryHeap().extract_min()


def test_string_elements():
    h = BinaryHeap()
    h.insert("pear")
    h.insert("apple")
    h.insert("mango")
    assert h.extract_min() == "apple"
