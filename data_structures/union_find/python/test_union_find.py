import pytest
from union_find import UnionFind


def test_initial():
    uf = UnionFind(5)
    assert uf.size() == 5
    assert uf.components() == 5
    assert not uf.connected(0, 1)


def test_unite():
    uf = UnionFind(5)
    uf.unite(0, 1)
    uf.unite(1, 2)
    assert uf.connected(0, 2)
    assert not uf.connected(0, 3)
    assert uf.components() == 3


def test_transitivity():
    uf = UnionFind(5)
    uf.unite(0, 1)
    uf.unite(2, 3)
    uf.unite(0, 3)
    assert uf.connected(1, 2)
    assert uf.components() == 2
    assert not uf.unite(0, 1)


def test_bad_element_raises():
    uf = UnionFind(3)
    with pytest.raises(IndexError):
        uf.find(5)
