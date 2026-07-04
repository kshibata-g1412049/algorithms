import pytest
from dynamic_array import DynamicArray


def test_new_size_zero():
    assert DynamicArray().size() == 0


def test_append_and_get():
    a = DynamicArray()
    a.append(10); a.append(20); a.append(30)
    assert a.get(0) == 10
    assert a.get(1) == 20
    assert a.get(2) == 30
    assert a.size() == 3


def test_set():
    a = DynamicArray()
    a.append(1); a.append(2); a.append(3)
    a.set(1, 99)
    assert a.get(1) == 99


def test_insert():
    a = DynamicArray()
    a.append(1); a.append(3)
    a.insert(1, 2)
    assert a.get(0) == 1
    assert a.get(1) == 2
    assert a.get(2) == 3
    assert a.size() == 3


def test_remove():
    a = DynamicArray()
    a.append(1); a.append(2); a.append(3)
    a.remove(1)
    assert a.get(0) == 1
    assert a.get(1) == 3
    assert a.size() == 2


def test_get_out_of_range():
    with pytest.raises(IndexError):
        DynamicArray().get(0)


def test_large_append():
    a = DynamicArray()
    for i in range(100):
        a.append(i)
    assert a.size() == 100
    assert a.get(50) == 50
    assert a.capacity() >= 100


def test_string_elements():
    a = DynamicArray()
    a.append("x")
    a.append("z")
    a.insert(1, "y")
    assert [a.get(i) for i in range(3)] == ["x", "y", "z"]
