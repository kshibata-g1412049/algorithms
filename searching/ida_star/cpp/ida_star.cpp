#include "ida_star.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <unordered_set>

namespace {

double octileDistance(std::pair<int, int> a, std::pair<int, int> b) {
    const double dx = std::abs(a.first - b.first);
    const double dy = std::abs(a.second - b.second);
    constexpr double kSqrt2Minus1 = 0.41421356237309515; // sqrt(2) - 1
    return std::max(dx, dy) + kSqrt2Minus1 * std::min(dx, dy);
}

// 8方向の移動オフセットと対応する移動コスト。
// 添字0-3が直進（上下左右）、4-7が斜め移動。
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
// 両側の直交マス（行方向のマスと列方向のマス）のいずれかが
// 障害物または範囲外であれば、その斜め移動はできない。
bool canMove(const std::vector<std::vector<int>>& grid, int row, int col, int dr, int dc,
             int numRows, int numCols) {
    const int newRow = row + dr;
    const int newCol = col + dc;
    if (!inBounds(newRow, newCol, numRows, numCols) || !isWalkable(grid, newRow, newCol)) {
        return false;
    }

    if (dr != 0 && dc != 0) {
        // 斜め移動: 両側の直交マスを確認する。
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

// セル座標(row,col)を一意な整数キーにエンコードする。
// 現在のDFSパス上に既にあるノードを検出するための集合のキーとして使う。
long long encode(int row, int col, int numCols) {
    return static_cast<long long>(row) * numCols + col;
}

constexpr double kFound = -1.0; // ゴールに到達したことを示すセンチネル値

// 現在のDFSパスpathの末尾ノードから探索を1段進める。
//
// 戻り値:
//   kFound      ゴールに到達した（pathはゴールまでの経路を保持したまま）。
//   kFound以外  このブランチで枝刈りされた場合に観測された最小のf値
//               （次の反復のboundの候補）。何も枝刈りされなければ
//               +infinityを返す。
double search(const std::vector<std::vector<int>>& grid, std::vector<std::pair<int, int>>& path,
              std::unordered_set<long long>& onPath, double g, double bound,
              std::pair<int, int> goal, int numRows, int numCols) {
    const std::pair<int, int> node = path.back();
    const double f = g + octileDistance(node, goal);
    if (f > bound) {
        return f; // この先は探索しても無駄: 上限を超えた候補値として返す
    }
    if (node == goal) {
        return kFound;
    }

    double minExceeded = std::numeric_limits<double>::infinity();

    for (int d = 0; d < kDirCount; ++d) {
        const int dr = kDirRow[d];
        const int dc = kDirCol[d];
        if (!canMove(grid, node.first, node.second, dr, dc, numRows, numCols)) {
            continue;
        }

        const std::pair<int, int> neighbor = {node.first + dr, node.second + dc};
        const long long neighborKey = encode(neighbor.first, neighbor.second, numCols);
        if (onPath.count(neighborKey) > 0) {
            continue; // 現在のDFSパス上に既にあるノードは再訪問しない（巡回防止）
        }

        path.push_back(neighbor);
        onPath.insert(neighborKey);

        const double result =
            search(grid, path, onPath, g + stepCost(dr, dc), bound, goal, numRows, numCols);
        if (result == kFound) {
            return kFound; // 見つかった経路はpathに残したまま即座に伝播する
        }

        path.pop_back();
        onPath.erase(neighborKey);
        minExceeded = std::min(minExceeded, result);
    }

    return minExceeded;
}

} // namespace

std::vector<std::pair<int, int>> idaStar(const std::vector<std::vector<int>>& grid,
                                          std::pair<int, int> start, std::pair<int, int> goal) {
    const int numRows = static_cast<int>(grid.size());
    const int numCols = numRows > 0 ? static_cast<int>(grid[0].size()) : 0;
    if (numRows == 0 || numCols == 0) {
        return {};
    }

    if (start == goal) {
        return {start};
    }

    double bound = octileDistance(start, goal);

    std::vector<std::pair<int, int>> path = {start};
    std::unordered_set<long long> onPath = {encode(start.first, start.second, numCols)};

    while (true) {
        const double result = search(grid, path, onPath, 0.0, bound, goal, numRows, numCols);
        if (result == kFound) {
            return path;
        }
        if (!std::isfinite(result)) {
            return {}; // どの分岐も上限を超えなかった = ゴールは到達不可能
        }
        bound = result;
    }
}
