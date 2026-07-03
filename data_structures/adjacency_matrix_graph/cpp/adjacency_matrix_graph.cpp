#include "adjacency_matrix_graph.h"

AdjacencyMatrixGraph::AdjacencyMatrixGraph(int n)
    : n_(n), matrix_(n, std::vector<bool>(n, false)) {}

void AdjacencyMatrixGraph::check(int v) const {
    if (v < 0 || v >= n_) throw std::out_of_range("vertex out of range");
}

void AdjacencyMatrixGraph::add_edge(int u, int v) {
    check(u); check(v);
    if (!matrix_[u][v]) { matrix_[u][v] = true; ++edge_count_; }
}

void AdjacencyMatrixGraph::remove_edge(int u, int v) {
    check(u); check(v);
    if (matrix_[u][v]) { matrix_[u][v] = false; --edge_count_; }
}

bool AdjacencyMatrixGraph::has_edge(int u, int v) const {
    check(u); check(v);
    return matrix_[u][v];
}

std::vector<int> AdjacencyMatrixGraph::get_neighbors(int v) const {
    check(v);
    std::vector<int> result;
    for (int i = 0; i < n_; ++i)
        if (matrix_[v][i]) result.push_back(i);
    return result;
}

int AdjacencyMatrixGraph::vertex_count() const { return n_; }
int AdjacencyMatrixGraph::edge_count() const { return edge_count_; }
