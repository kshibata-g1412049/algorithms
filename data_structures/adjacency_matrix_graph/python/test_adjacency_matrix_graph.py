import pytest
from adjacency_matrix_graph import AdjacencyMatrixGraph


def test_init():
    g = AdjacencyMatrixGraph(4)
    assert g.vertex_count() == 4
    assert g.edge_count() == 0


def test_add_and_has_edge():
    g = AdjacencyMatrixGraph(4)
    g.add_edge(0, 1)
    g.add_edge(0, 2)
    g.add_edge(1, 3)
    assert g.has_edge(0, 1)
    assert g.has_edge(0, 2)
    assert not g.has_edge(1, 0)
    assert g.edge_count() == 3
    assert g.get_neighbors(0) == [1, 2]


def test_remove_edge():
    g = AdjacencyMatrixGraph(4)
    g.add_edge(0, 1)
    g.add_edge(0, 2)
    g.remove_edge(0, 1)
    assert not g.has_edge(0, 1)
    assert g.edge_count() == 1
    g.add_edge(0, 1)
    assert g.edge_count() == 2


def test_bad_vertex_raises():
    g = AdjacencyMatrixGraph(4)
    with pytest.raises(IndexError):
        g.has_edge(0, 5)
