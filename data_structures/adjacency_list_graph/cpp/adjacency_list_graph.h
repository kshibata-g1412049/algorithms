#ifndef ADJACENCY_LIST_GRAPH_H
#define ADJACENCY_LIST_GRAPH_H

// 隣接リストグラフ（有向グラフ）
//
// 操作:
//   add_vertex(v)        : O(1)
//   add_edge(u, v)       : O(1)
//   has_edge(u, v)       : O(deg(u))
//   get_neighbors(v)     : O(1) — 参照返し
//   vertex_count()       : O(1)
//   edge_count()         : O(1)
//
// 頂点IDは任意の非負整数。存在しない頂点は add_vertex で追加。

#include <stdexcept>
#include <unordered_map>
#include <vector>

class AdjacencyListGraph {
public:
    void add_vertex(int v);
    void add_edge(int u, int v);
    bool has_edge(int u, int v) const;
    const std::vector<int>& get_neighbors(int v) const;
    bool has_vertex(int v) const;
    int vertex_count() const;
    int edge_count() const;

private:
    std::unordered_map<int, std::vector<int>> adj_;
    int edge_count_ = 0;
};

#endif // ADJACENCY_LIST_GRAPH_H
