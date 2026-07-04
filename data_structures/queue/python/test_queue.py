import pytest
from queue import Queue


def test_new_is_empty():
    assert Queue().is_empty()


def test_new_size_zero():
    assert Queue().size() == 0


def test_fifo_order():
    q = Queue()
    q.enqueue(1); q.enqueue(2); q.enqueue(3)
    assert q.dequeue() == 1
    assert q.dequeue() == 2
    assert q.dequeue() == 3
    assert q.is_empty()


def test_peek_no_remove():
    q = Queue()
    q.enqueue(5)
    assert q.peek() == 5
    assert q.size() == 1


def test_dequeue_empty_raises():
    with pytest.raises(IndexError):
        Queue().dequeue()


def test_peek_empty_raises():
    with pytest.raises(IndexError):
        Queue().peek()


def test_large_queue():
    q = Queue()
    for i in range(100):
        q.enqueue(i)
    assert q.size() == 100
    for i in range(100):
        assert q.dequeue() == i
    assert q.is_empty()


def test_string_elements():
    q = Queue()
    q.enqueue("first")
    q.enqueue("second")
    assert q.dequeue() == "first"
    assert q.dequeue() == "second"
