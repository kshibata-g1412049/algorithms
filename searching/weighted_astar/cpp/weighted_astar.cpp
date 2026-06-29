#include "weighted_astar.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>
#include <unordered_map>

namespace {

long long encode(int row, int col, int numCols) {
    return static_cast<long long>(row) * numCols + col;
}

// Octile distance（8方向移動に対する許容的ヒューリスティック）。
double octileDistance(std::pair<int, int> a, std::pair<int, int> b) {
    const double dx = std::abs(a.first - b.first);
    const double dy = std::abs(a.second - b.second);
    constexpr double kSqrt2Minus1 = 0.41421356237309515; // sqrt(2) - 1
    return std::max(dx, dy) + kSqrt2Minus1 * std::min(dx, dy);
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

std::vector<std::pair<int, int>> weightedAstar(const std::vector<std::vector<int>>& grid,
                                                std::pair<int, int> start,
                                                std::pair<int, int> goal, double weight) {
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

    std::unordered_map<long long, double> gScore;
    std::unordered_map<long long, long long> parent;
    std::unordered_map<long long, bool> closed;

    const long long startKey = encode(start.first, start.second, numCols);
    const long long goalKey = encode(goal.first, goal.second, numCols);

    gScore[startKey] = 0.0;
    openQueue.push({weight * octileDistance(start, goal), startKey});

    while (!openQueue.empty()) {
        const auto [currentF, currentKey] = openQueue.top();
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
        const double currentG = gScore[currentKey];

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

            const double tentativeG = currentG + stepCost(dr, dc);
            auto it = gScore.find(neighborKey);
            if (it == gScore.end() || tentativeG < it->second) {
                gScore[neighborKey] = tentativeG;
                parent[neighborKey] = currentKey;
                // f(n) = g(n) + weight * h(n)：ヒューリスティックの影響を
                // weight倍に強めることで探索を加速する（最適性は失われる）。
                const double f = tentativeG + weight * octileDistance({newRow, newCol}, goal);
                openQueue.push({f, neighborKey});
            }
        }
    }

    if (gScore.find(goalKey) == gScore.end() || !closed[goalKey]) {
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
