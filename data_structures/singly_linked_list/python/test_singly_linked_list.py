import pytest
from singly_linked_list import SinglyLinkedList


def test_new_is_empty():
    ll = SinglyLinkedList()
    assert ll.is_empty()
    assert ll.size() == 0


def test_push_front_order():
    ll = SinglyLinkedList()
    ll.push_front(3)
    ll.push_front(2)
    ll.push_front(1)
    assert ll.to_list() == [1, 2, 3]


def test_push_back_order():
    ll = SinglyLinkedList()
    ll.push_back(1)
    ll.push_back(2)
    ll.push_back(3)
    assert ll.to_list() == [1, 2, 3]


def test_front_back():
    ll = SinglyLinkedList()
    ll.push_back(1)
    ll.push_back(2)
    ll.push_back(3)
    assert ll.front() == 1
    assert ll.back() == 3


def test_pop_front():
    ll = SinglyLinkedList()
    ll.push_back(1)
    ll.push_back(2)
    ll.push_back(3)
    assert ll.pop_front() == 1
    assert ll.front() == 2
    assert ll.size() == 2


def test_contains():
    ll = SinglyLinkedList()
    ll.push_back(1)
    ll.push_back(2)
    ll.push_back(3)
    assert ll.contains(2)
    assert not ll.contains(5)


def test_remove_middle():
    ll = SinglyLinkedList()
    ll.push_back(1)
    ll.push_back(2)
    ll.push_back(3)
    assert ll.remove(2)
    assert ll.to_list() == [1, 3]
    assert not ll.remove(99)


def test_remove_head():
    ll = SinglyLinkedList()
    ll.push_back(1)
    ll.push_back(2)
    assert ll.remove(1)
    assert ll.to_list() == [2]


def test_front_empty_raises():
    ll = SinglyLinkedList()
    with pytest.raises(IndexError):
        ll.front()


def test_back_empty_raises():
    ll = SinglyLinkedList()
    with pytest.raises(IndexError):
        ll.back()


def test_pop_front_empty_raises():
    ll = SinglyLinkedList()
    with pytest.raises(IndexError):
        ll.pop_front()


def test_size_tracking():
    ll = SinglyLinkedList()
    for i in range(10):
        ll.push_back(i)
    assert ll.size() == 10
    for _ in range(5):
        ll.pop_front()
    assert ll.size() == 5
