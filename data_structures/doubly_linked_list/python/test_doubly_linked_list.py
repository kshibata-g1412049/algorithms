import pytest
from doubly_linked_list import DoublyLinkedList


def test_new_is_empty():
    ll = DoublyLinkedList()
    assert ll.is_empty()
    assert ll.size() == 0


def test_push_front_order():
    ll = DoublyLinkedList()
    ll.push_front(3)
    ll.push_front(2)
    ll.push_front(1)
    assert ll.to_list() == [1, 2, 3]


def test_push_back_order():
    ll = DoublyLinkedList()
    ll.push_back(1)
    ll.push_back(2)
    ll.push_back(3)
    assert ll.to_list() == [1, 2, 3]


def test_front_back():
    ll = DoublyLinkedList()
    ll.push_back(1)
    ll.push_back(2)
    ll.push_back(3)
    assert ll.front() == 1
    assert ll.back() == 3


def test_pop_front_and_back():
    ll = DoublyLinkedList()
    ll.push_back(1)
    ll.push_back(2)
    ll.push_back(3)
    assert ll.pop_front() == 1
    assert ll.front() == 2
    assert ll.pop_back() == 3
    assert ll.back() == 2
    assert ll.size() == 1


def test_contains():
    ll = DoublyLinkedList()
    ll.push_back(1)
    ll.push_back(2)
    ll.push_back(3)
    assert ll.contains(2)
    assert not ll.contains(5)


def test_remove():
    ll = DoublyLinkedList()
    ll.push_back(1)
    ll.push_back(2)
    ll.push_back(3)
    assert ll.remove(2)
    assert ll.to_list() == [1, 3]
    assert ll.remove(1)
    assert ll.to_list() == [3]
    assert not ll.remove(99)


def test_front_empty_raises():
    with pytest.raises(IndexError):
        DoublyLinkedList().front()


def test_pop_back_empty_raises():
    with pytest.raises(IndexError):
        DoublyLinkedList().pop_back()
