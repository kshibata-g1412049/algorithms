import pytest
from adjacency_list_graph import AdjacencyListGraph


def test_add_vertex():
    g = AdjacencyListGraph()
    g.add_vertex(0)
    g.add_vertex(1)
    g.add_vertex(2)
    assert g.vertex_count() == 3
    assert g.edge_count() == 0


def test_add_edge():
    g = AdjacencyListGraph()
    g.add_edge(0, 1)
    g.add_edge(0, 2)
    g.add_edge(1, 2)
    assert g.has_edge(0, 1)
    assert g.has_edge(0, 2)
    assert not g.has_edge(2, 0)
    assert g.edge_count() == 3
    assert g.vertex_count() == 3
    nb = g.get_neighbors(0)
    assert 1 in nb and 2 in nb


def test_auto_creates_vertices():
    g = AdjacencyListGraph()
    g.add_edge(5, 10)
    assert g.has_vertex(5)
    assert g.has_vertex(10)


def test_bad_vertex_raises():
    g = AdjacencyListGraph()
    with pytest.raises(KeyError):
        g.get_neighbors(99)
