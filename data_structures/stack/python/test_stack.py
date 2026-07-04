import pytest
from stack import Stack


def test_new_stack_is_empty():
    assert Stack().is_empty()


def test_new_stack_size_zero():
    assert Stack().size() == 0


def test_push_peek():
    s = Stack()
    s.push(10)
    assert s.peek() == 10
    assert s.size() == 1
    assert not s.is_empty()


def test_lifo_order():
    s = Stack()
    s.push(1); s.push(2); s.push(3)
    assert s.pop() == 3
    assert s.pop() == 2
    assert s.pop() == 1
    assert s.is_empty()


def test_peek_does_not_remove():
    s = Stack()
    s.push(5); s.push(7)
    assert s.peek() == 7
    assert s.size() == 2


def test_pop_empty_raises():
    with pytest.raises(IndexError):
        Stack().pop()


def test_peek_empty_raises():
    with pytest.raises(IndexError):
        Stack().peek()


def test_large_push_pop():
    s = Stack()
    for i in range(100):
        s.push(i)
    assert s.size() == 100
    assert s.peek() == 99
    for i in range(99, -1, -1):
        assert s.pop() == i
    assert s.is_empty()


def test_string_and_float_elements():
    s = Stack()
    s.push("hello")
    s.push("world")
    assert s.pop() == "world"
    f = Stack()
    f.push(3.14)
    assert f.peek() == 3.14
