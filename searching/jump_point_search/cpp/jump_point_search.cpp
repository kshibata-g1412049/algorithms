#include "jump_point_search.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <optional>
#include <queue>
#include <unordered_map>

namespace {

long long encode(int row, int col, int numCols) {
    return static_cast<long long>(row) * numCols + col;
}

double octileDistance(std::pair<int, int> a, std::pair<int, int> b) {
    const double dx = std::abs(a.first - b.first);
    const double dy = std::abs(a.second - b.second);
    constexpr double kSqrt2Minus1 = 0.41421356237309515; // sqrt(2) - 1
    return std::max(dx, dy) + kSqrt2Minus1 * std::min(dx, dy);
}

double stepCost(int dr, int dc) {
    constexpr double kSqrt2 = 1.4142135623730951;
    return (dr != 0 && dc != 0) ? kSqrt2 : 1.0;
}

bool inBounds(int row, int col, int numRows, int numCols) {
    return row >= 0 && row < numRows && col >= 0 && col < numCols;
}

bool isWalkable(const std::vector<std::vector<int>>& grid, int row, int col) {
    return grid[row][col] == 0;
}

// 隣接セル(row,col)から方向(dr,dc)へ進めるかどうかを判定する（コーナーカット禁止含む）。
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

constexpr int kAllDirRow[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
constexpr int kAllDirCol[8] = {0, 0, -1, 1, -1, 1, -1, 1};

// entry方向(dr,dc)に対する自然近傍（直進・斜め進入の主成分）かどうか。
bool isNaturalDirection(int dr, int dc, int ddr, int ddc) {
    if (dr != 0 && dc != 0) {
        return (ddr == dr && ddc == 0) || (ddr == 0 && ddc == dc) || (ddr == dr && ddc == dc);
    }
    return ddr == dr && ddc == dc;
}

// (row,col)に方向(dr,dc)で進入した際の強制近傍方向の一覧を返す。
//
// Harabor & Grastienによる強制近傍の定義「1手前のセルからは到達できな
// かった近傍が、現在のセルからは到達できる」を、コーナーカット禁止を
// 含むcanMoveに対してそのまま適用する。これにより、コーナーカット許容
// 前提でハードコードされた教科書通りの障害物判定（側方セルが障害物なら
// 強制近傍、等）に頼らずに済む。コーナーカット禁止下ではその判定条件と
// 提案される斜め移動が両立しない（判定に使う障害物セル自体がcanMoveの
// コーナーカット判定対象セルと一致してしまう）ことがあり、教科書の式を
// そのまま転用すると必要な近傍を取り逃す。1手前との比較に基づく本定義は
// この問題を構造的に回避する。
// 後退方向（来た方向の正反対）のみを除外し、その他の7方向は新規性
// （1手前では不可、現在は可能）のみで判定する。
std::vector<std::pair<int, int>> forcedNeighborDirections(const std::vector<std::vector<int>>& grid,
                                                            int row, int col, int dr, int dc,
                                                            int numRows, int numCols) {
    const int prevRow = row - dr;
    const int prevCol = col - dc;
    const bool prevInBounds = inBounds(prevRow, prevCol, numRows, numCols);

    std::vector<std::pair<int, int>> result;
    for (int d = 0; d < 8; ++d) {
        const int ddr = kAllDirRow[d];
        const int ddc = kAllDirCol[d];
        if (isNaturalDirection(dr, dc, ddr, ddc)) {
            continue;
        }
        if (ddr == -dr && ddc == -dc) {
            continue; // 正反対の後退方向は最短経路に寄与しないため除外
        }
        if (!canMove(grid, row, col, ddr, ddc, numRows, numCols)) {
            continue;
        }
        const bool prevCouldMove =
            prevInBounds && canMove(grid, prevRow, prevCol, ddr, ddc, numRows, numCols);
        if (!prevCouldMove) {
            result.push_back({ddr, ddc});
        }
    }
    return result;
}

bool hasForcedNeighbor(const std::vector<std::vector<int>>& grid, int row, int col, int dr,
                        int dc, int numRows, int numCols) {
    return !forcedNeighborDirections(grid, row, col, dr, dc, numRows, numCols).empty();
}

// 方向(dr,dc)へ直進し続け、ジャンプポイント（強制近傍を持つセル・ゴール・
// それ以上進めない行き止まりのセル）を探す。最初の一歩目で進めない場合
// （すぐに障害物・範囲外に当たった場合）のみstd::nulloptを返す。
// 1歩以上進めた後に行き止まりになった場合は、その地点を曲がるための
// ジャンプポイントとして返す（グリッド境界で直角に曲がる必要がある経路を
// 取り逃さないため。これは強制近傍が存在する場合と同様に扱ってよい —
// その方向にこれ以上直進できないという事実そのものが、探索を分岐させる
// べき理由になる）。
std::optional<std::pair<int, int>> jump(const std::vector<std::vector<int>>& grid, int row,
                                         int col, int dr, int dc, std::pair<int, int> goal,
                                         int numRows, int numCols) {
    if (!canMove(grid, row, col, dr, dc, numRows, numCols)) {
        return std::nullopt;
    }

    int curRow = row + dr;
    int curCol = col + dc;

    while (true) {
        if (std::make_pair(curRow, curCol) == goal) {
            return std::make_pair(curRow, curCol);
        }

        if (hasForcedNeighbor(grid, curRow, curCol, dr, dc, numRows, numCols)) {
            return std::make_pair(curRow, curCol);
        }

        if (dr != 0 && dc != 0) {
            // 斜め移動の場合、各ステップで水平・垂直方向のサブジャンプも試す。
            if (jump(grid, curRow, curCol, dr, 0, goal, numRows, numCols).has_value()) {
                return std::make_pair(curRow, curCol);
            }
            if (jump(grid, curRow, curCol, 0, dc, goal, numRows, numCols).has_value()) {
                return std::make_pair(curRow, curCol);
            }
        }

        if (!canMove(grid, curRow, curCol, dr, dc, numRows, numCols)) {
            return std::make_pair(curRow, curCol);
        }
        curRow += dr;
        curCol += dc;
    }
}

// 現在ノードについて、探索すべき方向(dr,dc)の集合を刈り込んで返す。
// parentがnulloptの場合（start地点）は8方向すべてを候補とする。
std::vector<std::pair<int, int>> prunedDirections(const std::vector<std::vector<int>>& grid,
                                                    int row, int col,
                                                    std::optional<std::pair<int, int>> parent,
                                                    int numRows, int numCols) {
    std::vector<std::pair<int, int>> dirs;

    if (!parent.has_value()) {
        for (int d = 0; d < 8; ++d) {
            if (canMove(grid, row, col, kAllDirRow[d], kAllDirCol[d], numRows, numCols)) {
                dirs.push_back({kAllDirRow[d], kAllDirCol[d]});
            }
        }
        return dirs;
    }

    const int dr = [&] {
        const int diff = row - parent->first;
        return (diff > 0) - (diff < 0);
    }();
    const int dc = [&] {
        const int diff = col - parent->second;
        return (diff > 0) - (diff < 0);
    }();

    // 自然近傍（直進方向、斜め進入なら水平・垂直成分も含む）。
    if (dr != 0 && dc != 0) {
        if (canMove(grid, row, col, dr, 0, numRows, numCols)) {
            dirs.push_back({dr, 0});
        }
        if (canMove(grid, row, col, 0, dc, numRows, numCols)) {
            dirs.push_back({0, dc});
        }
    }
    if (canMove(grid, row, col, dr, dc, numRows, numCols)) {
        dirs.push_back({dr, dc});
    }

    // 強制近傍（hasForcedNeighborと同じ「1手前では不可、現在は可能」の定義）。
    for (const auto& d : forcedNeighborDirections(grid, row, col, dr, dc, numRows, numCols)) {
        dirs.push_back(d);
    }

    return dirs;
}

} // namespace

std::vector<std::pair<int, int>> jumpPointSearch(const std::vector<std::vector<int>>& grid,
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

    const long long startKey = encode(start.first, start.second, numCols);
    const long long goalKey = encode(goal.first, goal.second, numCols);

    using QueueEntry = std::pair<double, long long>;
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, std::greater<QueueEntry>> openQueue;

    std::unordered_map<long long, double> gScore;
    std::unordered_map<long long, long long> parent; // ジャンプポイント間の親（飛び先の起点）
    std::unordered_map<long long, bool> closed;

    gScore[startKey] = 0.0;
    openQueue.push({octileDistance(start, goal), startKey});

    bool found = false;

    while (!openQueue.empty()) {
        const auto [currentF, currentKey] = openQueue.top();
        openQueue.pop();

        if (closed[currentKey]) {
            continue;
        }
        closed[currentKey] = true;

        if (currentKey == goalKey) {
            found = true;
            break;
        }

        const int row = static_cast<int>(currentKey / numCols);
        const int col = static_cast<int>(currentKey % numCols);

        std::optional<std::pair<int, int>> parentPos;
        auto parentIt = parent.find(currentKey);
        if (parentIt != parent.end()) {
            const long long parentKey = parentIt->second;
            parentPos = {static_cast<int>(parentKey / numCols),
                         static_cast<int>(parentKey % numCols)};
        }

        const auto dirs = prunedDirections(grid, row, col, parentPos, numRows, numCols);
        const double currentG = gScore[currentKey];

        for (const auto& [dr, dc] : dirs) {
            const auto jp = jump(grid, row, col, dr, dc, goal, numRows, numCols);
            if (!jp.has_value()) {
                continue;
            }
            const long long jpKey = encode(jp->first, jp->second, numCols);
            if (closed[jpKey]) {
                continue;
            }

            const int steps = std::max(std::abs(jp->first - row), std::abs(jp->second - col));
            const double tentativeG = currentG + steps * stepCost(dr, dc);

            auto it = gScore.find(jpKey);
            if (it == gScore.end() || tentativeG < it->second) {
                gScore[jpKey] = tentativeG;
                parent[jpKey] = currentKey;
                const double f = tentativeG + octileDistance(*jp, goal);
                openQueue.push({f, jpKey});
            }
        }
    }

    if (!found) {
        return {}; // 到達不可能
    }

    // ジャンプポイントの列をたどって経路を復元する。
    std::vector<std::pair<int, int>> jumpPoints;
    long long currentKey = goalKey;
    while (true) {
        const int row = static_cast<int>(currentKey / numCols);
        const int col = static_cast<int>(currentKey % numCols);
        jumpPoints.push_back({row, col});
        if (currentKey == startKey) {
            break;
        }
        currentKey = parent[currentKey];
    }
    std::reverse(jumpPoints.begin(), jumpPoints.end());

    // ジャンプポイント間は直線（直進または斜め45度）でつながっているため、
    // 1セルずつ展開して連続経路を構築する。
    std::vector<std::pair<int, int>> path;
    path.push_back(jumpPoints.front());
    for (size_t i = 1; i < jumpPoints.size(); ++i) {
        const auto& [fromRow, fromCol] = jumpPoints[i - 1];
        const auto& [toRow, toCol] = jumpPoints[i];
        const int dr = (toRow > fromRow) - (toRow < fromRow);
        const int dc = (toCol > fromCol) - (toCol < fromCol);
        int curRow = fromRow;
        int curCol = fromCol;
        while (curRow != toRow || curCol != toCol) {
            curRow += dr;
            curCol += dc;
            path.push_back({curRow, curCol});
        }
    }

    return path;
}
