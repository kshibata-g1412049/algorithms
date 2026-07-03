import pytest
from deque import Deque


def test_new_is_empty():
    assert Deque().is_empty()


def test_push_back_and_peek():
    d = Deque()
    d.push_back(1); d.push_back(2); d.push_back(3)
    assert d.peek_front() == 1
    assert d.peek_back() == 3
    assert d.size() == 3


def test_push_front():
    d = Deque()
    d.push_front(3); d.push_front(2); d.push_front(1)
    assert d.pop_front() == 1
    assert d.pop_front() == 2
    assert d.pop_front() == 3


def test_pop_back():
    d = Deque()
    d.push_back(10); d.push_back(20)
    assert d.pop_back() == 20
    assert d.pop_back() == 10


def test_mixed_operations():
    d = Deque()
    d.push_front(2); d.push_front(1); d.push_back(3); d.push_back(4)
    assert d.size() == 4
    assert d.pop_front() == 1
    assert d.pop_back() == 4


def test_pop_front_empty_raises():
    with pytest.raises(IndexError):
        Deque().pop_front()


def test_pop_back_empty_raises():
    with pytest.raises(IndexError):
        Deque().pop_back()


def test_large_deque():
    d = Deque()
    for i in range(50):
        d.push_front(i)
        d.push_back(100 + i)
    assert d.size() == 100
