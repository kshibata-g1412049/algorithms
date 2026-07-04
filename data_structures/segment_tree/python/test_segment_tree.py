import pytest
from segment_tree import SegmentTree


def test_sum_queries():
    data = [1, 3, 5, 7, 9, 11]
    st = SegmentTree(data)
    assert st.size() == 6
    assert st.query(0, 6) == 36
    assert st.query(1, 4) == 15
    assert st.query(2, 3) == 5


def test_update():
    data = [1, 3, 5, 7, 9, 11]
    st = SegmentTree(data)
    st.update(2, 10)
    assert st.query(0, 6) == 41
    assert st.query(1, 4) == 20


def test_brute_force():
    data = list(range(1, 101))
    st = SegmentTree(data)
    assert st.query(20, 50) == sum(data[20:50])
    data[30] = 999
    st.update(30, 999)
    assert st.query(20, 50) == sum(data[20:50])


def test_bad_index_raises():
    st = SegmentTree([1, 2, 3])
    with pytest.raises(IndexError):
        st.update(5, 0)
    with pytest.raises(IndexError):
        st.query(2, 1)


def test_float_elements():
    st = SegmentTree([0.5, 1.5, 2.0])
    assert st.query(0, 3) == 4.0
    st.update(1, 2.5)
    assert st.query(0, 3) == 5.0
