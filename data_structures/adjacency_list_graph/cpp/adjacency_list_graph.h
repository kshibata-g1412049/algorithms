#ifndef ADJACENCY_LIST_GRAPH_H
#define ADJACENCY_LIST_GRAPH_H

// 隣接リストグラフ（有向グラフ）（テンプレート版）
//
// 頂点IDは任意の型 VertexId（既定 int）。ハッシュは Hash（既定
// std::hash<VertexId>）で計算し、VertexId は == 比較可能であること。
// 文字列ラベルの頂点（AdjacencyListGraph<std::string>）等が使える。
//
// 操作:
//   add_vertex(v)        : O(1)
//   add_edge(u, v)       : O(1)
//   has_edge(u, v)       : O(deg(u))
//   get_neighbors(v)     : O(1) — 参照返し
//   vertex_count()       : O(1)
//   edge_count()         : O(1)
//
// 存在しない頂点は add_edge で自動追加。

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <vector>

template <typename VertexId = int, typename Hash = std::hash<VertexId>>
class AdjacencyListGraph {
public:
    void add_vertex(const VertexId& v) {
        adj_.emplace(v, std::vector<VertexId>{});
    }

    void add_edge(const VertexId& u, const VertexId& v) {
        if (!has_vertex(u)) add_vertex(u);
        if (!has_vertex(v)) add_vertex(v);
        adj_[u].push_back(v);
        ++edge_count_;
    }

    bool has_edge(const VertexId& u, const VertexId& v) const {
        auto it = adj_.find(u);
        if (it == adj_.end()) return false;
        return std::find(it->second.begin(), it->second.end(), v) != it->second.end();
    }

    const std::vector<VertexId>& get_neighbors(const VertexId& v) const {
        auto it = adj_.find(v);
        if (it == adj_.end()) throw std::out_of_range("vertex not found");
        return it->second;
    }

    bool has_vertex(const VertexId& v) const { return adj_.count(v) > 0; }
    int vertex_count() const { return static_cast<int>(adj_.size()); }
    int edge_count() const { return edge_count_; }

private:
    std::unordered_map<VertexId, std::vector<VertexId>, Hash> adj_;
    int edge_count_ = 0;
};

#endif // ADJACENCY_LIST_GRAPH_H
