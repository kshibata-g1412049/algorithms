import pytest
from circular_buffer import CircularBuffer


def test_new_is_empty():
    assert CircularBuffer(3).is_empty()


def test_capacity():
    assert CircularBuffer(5).capacity() == 5


def test_push_and_full():
    b = CircularBuffer(3)
    b.push(1); b.push(2); b.push(3)
    assert b.is_full()
    assert b.peek() == 1


def test_fifo_pop():
    b = CircularBuffer(3)
    b.push(1); b.push(2); b.push(3)
    assert b.pop() == 1
    assert b.pop() == 2
    assert b.pop() == 3
    assert b.is_empty()


def test_wrap_around():
    b = CircularBuffer(3)
    b.push(1); b.push(2); b.push(3)
    b.pop(); b.pop()
    b.push(4); b.push(5)
    assert b.peek() == 3
    assert b.size() == 3


def test_push_full_raises():
    b = CircularBuffer(2)
    b.push(1); b.push(2)
    with pytest.raises(OverflowError):
        b.push(3)


def test_pop_empty_raises():
    with pytest.raises(IndexError):
        CircularBuffer(2).pop()


def test_zero_capacity_raises():
    with pytest.raises(ValueError):
        CircularBuffer(0)
