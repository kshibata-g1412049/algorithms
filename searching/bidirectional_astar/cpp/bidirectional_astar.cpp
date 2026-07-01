#include "bidirectional_astar.h"

#include <algorithm>
#include <cmath>
#include <limits>
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

constexpr int kDirCount = 8;
constexpr int kDirRow[kDirCount] = {-1, 1, 0, 0, -1, -1, 1, 1};
constexpr int kDirCol[kDirCount] = {0, 0, -1, 1, -1, 1, -1, 1};

bool inBounds(int row, int col, int numRows, int numCols) {
    return row >= 0 && row < numRows && col >= 0 && col < numCols;
}

bool isWalkable(const std::vector<std::vector<int>>& grid, int row, int col) {
    return grid[row][col] == 0;
}

// 隣接セル(row,col)から方向dへ進めるかどうかを判定する（コーナーカット禁止含む）。
// 8方向移動は対称（A->Bが可能ならB->Aも可能）なので、前進・後退の両探索で
// 同じ関数をそのまま利用できる。
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

// 一方向分の探索状態（前進探索・後退探索どちらにも使う共通構造）。
struct SearchState {
    std::unordered_map<long long, double> gScore;
    std::unordered_map<long long, long long> parent;
    std::unordered_map<long long, bool> closed;
    using QueueEntry = std::pair<double, long long>;
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, std::greater<QueueEntry>> openQueue;
};

// SearchStateを1ステップ進める（キューの先頭を1つ展開する）。
// targetは探索のヒューリスティック目標（前進探索ならgoal、後退探索ならstart）。
// 展開したノードのキーをexpandedKeyに格納し、true/falseで展開が行われたかを返す。
bool expandOne(SearchState& state, const std::vector<std::vector<int>>& grid, int numRows,
                int numCols, std::pair<int, int> target, long long& expandedKey) {
    while (!state.openQueue.empty()) {
        const auto [currentF, currentKey] = state.openQueue.top();
        state.openQueue.pop();

        if (state.closed[currentKey]) {
            continue; // 古いエントリはスキップ
        }
        state.closed[currentKey] = true;
        expandedKey = currentKey;

        const int row = static_cast<int>(currentKey / numCols);
        const int col = static_cast<int>(currentKey % numCols);
        const double currentG = state.gScore[currentKey];

        for (int d = 0; d < kDirCount; ++d) {
            const int dr = kDirRow[d];
            const int dc = kDirCol[d];
            if (!canMove(grid, row, col, dr, dc, numRows, numCols)) {
                continue;
            }

            const int newRow = row + dr;
            const int newCol = col + dc;
            const long long neighborKey = encode(newRow, newCol, numCols);
            if (state.closed[neighborKey]) {
                continue;
            }

            const double tentativeG = currentG + stepCost(dr, dc);
            auto it = state.gScore.find(neighborKey);
            if (it == state.gScore.end() || tentativeG < it->second) {
                state.gScore[neighborKey] = tentativeG;
                state.parent[neighborKey] = currentKey;
                const double f = tentativeG + octileDistance({newRow, newCol}, target);
                state.openQueue.push({f, neighborKey});
            }
        }

        return true; // 1ノード展開した
    }
    return false; // キューが空 = これ以上展開できない
}

} // namespace

std::vector<std::pair<int, int>> bidirectionalAstar(const std::vector<std::vector<int>>& grid,
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

    SearchState forward;  // start -> goal方向
    SearchState backward; // goal -> start方向

    forward.gScore[startKey] = 0.0;
    forward.openQueue.push({octileDistance(start, goal), startKey});

    backward.gScore[goalKey] = 0.0;
    backward.openQueue.push({octileDistance(goal, start), goalKey});

    double bestCost = std::numeric_limits<double>::infinity();
    long long meetingKey = -1;

    // 交互に1ノードずつ展開する。あるノードが両方のclosed集合に含まれた
    // 時点でその経路長を候補とし、両側のキューの最小f値の合計が候補コスト
    // 以上になった時点で最適性が保証されるため探索を終了する。
    while (!forward.openQueue.empty() && !backward.openQueue.empty()) {
        // 標準的な停止条件: 両側の最小f値の合計が現在の最良コスト以上なら終了。
        const double forwardTopF = forward.openQueue.top().first;
        const double backwardTopF = backward.openQueue.top().first;
        if (forwardTopF + backwardTopF >= bestCost) {
            break;
        }

        long long expandedForwardKey = -1;
        if (expandOne(forward, grid, numRows, numCols, goal, expandedForwardKey)) {
            if (backward.gScore.count(expandedForwardKey)) {
                const double candidate =
                    forward.gScore[expandedForwardKey] + backward.gScore[expandedForwardKey];
                if (candidate < bestCost) {
                    bestCost = candidate;
                    meetingKey = expandedForwardKey;
                }
            }
        }

        if (forward.openQueue.empty() || backward.openQueue.empty()) {
            break;
        }

        long long expandedBackwardKey = -1;
        if (expandOne(backward, grid, numRows, numCols, start, expandedBackwardKey)) {
            if (forward.gScore.count(expandedBackwardKey)) {
                const double candidate =
                    forward.gScore[expandedBackwardKey] + backward.gScore[expandedBackwardKey];
                if (candidate < bestCost) {
                    bestCost = candidate;
                    meetingKey = expandedBackwardKey;
                }
            }
        }
    }

    if (meetingKey == -1) {
        return {}; // 到達不可能
    }

    // 前進側: start -> meetingKey
    std::vector<std::pair<int, int>> forwardPath;
    long long currentKey = meetingKey;
    while (true) {
        const int row = static_cast<int>(currentKey / numCols);
        const int col = static_cast<int>(currentKey % numCols);
        forwardPath.push_back({row, col});
        if (currentKey == startKey) {
            break;
        }
        currentKey = forward.parent[currentKey];
    }
    std::reverse(forwardPath.begin(), forwardPath.end());

    // 後退側: meetingKey -> goal（backward.parentはgoal方向への木なので
    // meetingKeyから辿るとgoalに到達する順序になる）
    std::vector<std::pair<int, int>> backwardPath;
    currentKey = meetingKey;
    while (currentKey != goalKey) {
        currentKey = backward.parent[currentKey];
        const int row = static_cast<int>(currentKey / numCols);
        const int col = static_cast<int>(currentKey % numCols);
        backwardPath.push_back({row, col});
    }

    // forwardPathの末尾がmeetingKey（重複）なので、backwardPathをそのまま連結する。
    forwardPath.insert(forwardPath.end(), backwardPath.begin(), backwardPath.end());
    return forwardPath;
}
