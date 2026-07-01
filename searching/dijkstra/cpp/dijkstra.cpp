#include "dijkstra.h"

#include <algorithm>
#include <cmath>
#include <queue>
#include <unordered_map>

namespace {

long long encode(int row, int col, int numCols) {
    return static_cast<long long>(row) * numCols + col;
}

constexpr int kDirCount = 8;
constexpr int kDirRow[kDirCount] = {-1, 1, 0, 0, -1, -1, 1, 1};
constexpr int kDirCol[kDirCount] = {0, 0, -1, 1, -1, 1, -1, 1};

bool inBounds(int row, int col, int numRows, int numCols) {
    return row >= 0 && row < numRows && col >= 0 && col < numCols;
}

bool isWalkable(const std::vector<std::vector<int>>& grid, int row, int col) {
    return grid[row][col] == 0;
}

// 隣接セル(row,col)から方向dへ進めるかどうかを判定する。
// 斜め移動の場合はコーナーカット禁止のルールを適用する：
// 両側の直交マスのいずれかが障害物または範囲外であれば斜め移動できない。
bool canMove(const std::vector<std::vector<int>>& grid, int row, int col, int dr, int dc,
             int numRows, int numCols) {
    const int newRow = row + dr;
    const int newCol = col + dc;
    if (!inBounds(newRow, newCol, numRows, numCols) || !isWalkable(grid, newRow, newCol)) {
        return false;
    }

    if (dr != 0 && dc != 0) {
        if (!inBounds(row + dr, col, numRows, numCols) || !isWalkable(grid, row + dr, col)) {
            return false;
        }
        if (!inBounds(row, col + dc, numRows, numCols) || !isWalkable(grid, row, col + dc)) {
            return false;
        }
    }

    return true;
}

double stepCost(int dr, int dc) {
    constexpr double kSqrt2 = 1.4142135623730951;
    return (dr != 0 && dc != 0) ? kSqrt2 : 1.0;
}

} // namespace

std::vector<std::pair<int, int>> dijkstra(const std::vector<std::vector<int>>& grid,
                                           std::pair<int, int> start,
                                           std::pair<int, int> goal) {
    const int numRows = static_cast<int>(grid.size());
    const int numCols = numRows > 0 ? static_cast<int>(grid[0].size()) : 0;
    if (numRows == 0 || numCols == 0) {
        return {};
    }

    if (start == goal) {
        return {start};
    }

    using QueueEntry = std::pair<double, long long>;
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, std::greater<QueueEntry>> openQueue;

    std::unordered_map<long long, double> dist;
    std::unordered_map<long long, long long> parent;
    std::unordered_map<long long, bool> closed;

    const long long startKey = encode(start.first, start.second, numCols);
    const long long goalKey = encode(goal.first, goal.second, numCols);

    dist[startKey] = 0.0;
    openQueue.push({0.0, startKey});

    while (!openQueue.empty()) {
        const auto [currentDist, currentKey] = openQueue.top();
        openQueue.pop();

        if (closed[currentKey]) {
            continue;
        }
        closed[currentKey] = true;

        if (currentKey == goalKey) {
            break;
        }

        const int row = static_cast<int>(currentKey / numCols);
        const int col = static_cast<int>(currentKey % numCols);

        for (int d = 0; d < kDirCount; ++d) {
            const int dr = kDirRow[d];
            const int dc = kDirCol[d];
            if (!canMove(grid, row, col, dr, dc, numRows, numCols)) {
                continue;
            }

            const int newRow = row + dr;
            const int newCol = col + dc;
            const long long neighborKey = encode(newRow, newCol, numCols);
            if (closed[neighborKey]) {
                continue;
            }

            const double tentativeDist = currentDist + stepCost(dr, dc);
            auto it = dist.find(neighborKey);
            if (it == dist.end() || tentativeDist < it->second) {
                dist[neighborKey] = tentativeDist;
                parent[neighborKey] = currentKey;
                openQueue.push({tentativeDist, neighborKey});
            }
        }
    }

    if (dist.find(goalKey) == dist.end() || !closed[goalKey]) {
        return {};
    }

    std::vector<std::pair<int, int>> path;
    long long currentKey = goalKey;
    while (true) {
        const int row = static_cast<int>(currentKey / numCols);
        const int col = static_cast<int>(currentKey % numCols);
        path.push_back({row, col});
        if (currentKey == startKey) {
            break;
        }
        currentKey = parent[currentKey];
    }
    std::reverse(path.begin(), path.end());
    return path;
}
