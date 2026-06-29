#include "dfs.h"

namespace {

// currentから再帰的に未訪問の隣接ノードをたどるヘルパー関数。
// 隣接リストの並び順で最初に見つかった未訪問ノードへ進む。
void visit(const std::vector<std::vector<int>>& adjacency, int current,
           std::vector<bool>& visited, std::vector<int>& order) {
    visited[current] = true;
    order.push_back(current);

    for (int neighbor : adjacency[current]) {
        if (!visited[neighbor]) {
            visit(adjacency, neighbor, visited, order);
        }
    }
}

}  // namespace

std::vector<int> dfs(const std::vector<std::vector<int>>& adjacency, int start) {
    const size_t n = adjacency.size();
    std::vector<int> order;

    if (start < 0 || static_cast<size_t>(start) >= n) {
        // 範囲外の開始ノードは訪問順序なしとして空を返す
        return order;
    }

    std::vector<bool> visited(n, false);
    visit(adjacency, start, visited, order);

    return order;
}
