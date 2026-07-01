#include "bfs.h"
#include <queue>

std::vector<int> bfs(const std::vector<std::vector<int>>& adjacency, int start) {
    const size_t n = adjacency.size();
    std::vector<int> order;

    if (start < 0 || static_cast<size_t>(start) >= n) {
        // 範囲外の開始ノードは訪問順序なしとして空を返す
        return order;
    }

    std::vector<bool> visited(n, false);
    std::queue<int> queue;

    visited[start] = true;
    queue.push(start);

    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();
        order.push_back(current);

        // 隣接リストの並び順で未訪問の隣接ノードをキューに追加する
        for (int neighbor : adjacency[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue.push(neighbor);
            }
        }
    }

    return order;
}
