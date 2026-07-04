import pytest
from hash_map import HashMap


def test_new_is_empty():
    m = HashMap()
    assert m.is_empty()
    assert m.size() == 0


def test_insert_and_get():
    m = HashMap()
    m.insert(1, 10)
    m.insert(2, 20)
    m.insert(3, 30)
    assert m.get(1) == 10
    assert m.get(3) == 30
    assert m.contains(2)
    assert not m.contains(5)
    assert m.size() == 3


def test_update():
    m = HashMap()
    m.insert(1, 10)
    m.insert(1, 99)
    assert m.get(1) == 99
    assert m.size() == 1


def test_remove():
    m = HashMap()
    m.insert(1, 10)
    m.insert(2, 20)
    assert m.remove(1)
    assert not m.contains(1)
    assert not m.remove(99)


def test_get_absent_raises():
    m = HashMap()
    with pytest.raises(KeyError):
        m.get(99)


def test_100_elements():
    m = HashMap()
    for i in range(100):
        m.insert(i, i * 2)
    assert all(m.get(i) == i * 2 for i in range(100))


def test_string_keys_and_values():
    m = HashMap()
    m.insert("apple", "red")
    m.insert("banana", "yellow")
    assert m.get("apple") == "red"
    assert m.contains("banana")
    assert not m.contains("cherry")
    assert m.remove("apple")
    assert m.size() == 1
