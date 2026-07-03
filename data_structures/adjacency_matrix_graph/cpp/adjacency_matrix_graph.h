#ifndef ADJACENCY_MATRIX_GRAPH_H
#define ADJACENCY_MATRIX_GRAPH_H

// 隣接行列グラフ（有向グラフ、固定頂点数）
//
// 操作:
//   add_edge(u, v)      : O(1)
//   remove_edge(u, v)   : O(1)
//   has_edge(u, v)      : O(1)
//   get_neighbors(v)    : O(V)
//   vertex_count()      : O(1)
//   edge_count()        : O(1)
//
// コンストラクタで頂点数を指定（頂点ID: 0～n-1）。
// 範囲外頂点は std::out_of_range。

#include <stdexcept>
#include <vector>

class AdjacencyMatrixGraph {
public:
    explicit AdjacencyMatrixGraph(int n);

    void add_edge(int u, int v);
    void remove_edge(int u, int v);
    bool has_edge(int u, int v) const;
    std::vector<int> get_neighbors(int v) const;
    int vertex_count() const;
    int edge_count() const;

private:
    int n_;
    std::vector<std::vector<bool>> matrix_;
    int edge_count_ = 0;

    void check(int v) const;
};

#endif // ADJACENCY_MATRIX_GRAPH_H
