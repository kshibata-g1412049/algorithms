#include "adjacency_list_graph.h"
#include <algorithm>

void AdjacencyListGraph::add_vertex(int v) {
    adj_.emplace(v, std::vector<int>{});
}

void AdjacencyListGraph::add_edge(int u, int v) {
    if (!has_vertex(u)) add_vertex(u);
    if (!has_vertex(v)) add_vertex(v);
    adj_[u].push_back(v);
    ++edge_count_;
}

bool AdjacencyListGraph::has_edge(int u, int v) const {
    auto it = adj_.find(u);
    if (it == adj_.end()) return false;
    return std::find(it->second.begin(), it->second.end(), v) != it->second.end();
}

const std::vector<int>& AdjacencyListGraph::get_neighbors(int v) const {
    auto it = adj_.find(v);
    if (it == adj_.end()) throw std::out_of_range("vertex not found");
    return it->second;
}

bool AdjacencyListGraph::has_vertex(int v) const { return adj_.count(v) > 0; }
int AdjacencyListGraph::vertex_count() const { return static_cast<int>(adj_.size()); }
int AdjacencyListGraph::edge_count() const { return edge_count_; }
