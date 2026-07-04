import pytest
from fenwick_tree import FenwickTree


def test_manual_update():
    ft = FenwickTree(6)
    for i in range(6):
        ft.update(i, i + 1)
    assert ft.prefix_sum(6) == 21
    assert ft.range_sum(1, 4) == 9


def test_from_data():
    data = [1, 3, 5, 7, 9, 11]
    ft = FenwickTree(data)
    assert ft.prefix_sum(6) == 36
    assert ft.range_sum(1, 4) == 15
    ft.update(2, 5)
    assert ft.range_sum(1, 4) == 20


def test_brute_force():
    data = list(range(1, 101))
    ft = FenwickTree(data)
    assert ft.range_sum(20, 50) == sum(data[20:50])
    ft.update(30, 900)
    data[30] += 900
    assert ft.range_sum(20, 50) == sum(data[20:50])


def test_bad_index_raises():
    ft = FenwickTree(3)
    with pytest.raises(IndexError):
        ft.update(5, 1)
    with pytest.raises(IndexError):
        ft.range_sum(2, 1)


def test_float_elements():
    ft = FenwickTree(3)
    ft.update(0, 0.5)
    ft.update(1, 1.5)
    ft.update(2, 2.0)
    assert ft.prefix_sum(3) == 4.0
    assert ft.range_sum(1, 3) == 3.5
