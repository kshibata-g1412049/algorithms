// jump_point_search.cppのテスト。
//
// このファイルは同じフォルダ内のjump_point_search.h/.cppのみに依存しており、
// 外部の共有ヘッダを使わない。そのためcpp/フォルダを単独で取り出しても
// `cmake .. && make && ctest` でビルド・実行できる。
#include "jump_point_search.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace {

constexpr double kEps = 1e-6;
constexpr double kSqrt2 = 1.4142135623730951;

double pathCost(const std::vector<std::pair<int, int>>& path) {
    double cost = 0.0;
    for (size_t i = 1; i < path.size(); ++i) {
        const int dr = std::abs(path[i].first - path[i - 1].first);
        const int dc = std::abs(path[i].second - path[i - 1].second);
        cost += (dr == 1 && dc == 1) ? kSqrt2 : 1.0;
    }
    return cost;
}

bool isValidPath(const std::vector<std::vector<int>>& grid,
                  const std::vector<std::pair<int, int>>& path, std::pair<int, int> start,
                  std::pair<int, int> goal) {
    if (path.empty()) {
        return false;
    }
    if (path.front() != start || path.back() != goal) {
        return false;
    }

    for (const auto& [r, c] : path) {
        if (r < 0 || r >= static_cast<int>(grid.size()) || c < 0 ||
            c >= static_cast<int>(grid[0].size())) {
            return false;
        }
        if (grid[r][c] != 0) {
            return false;
        }
    }

    for (size_t i = 1; i < path.size(); ++i) {
        const int dr = path[i].first - path[i - 1].first;
        const int dc = path[i].second - path[i - 1].second;
        if (std::abs(dr) > 1 || std::abs(dc) > 1 || (dr == 0 && dc == 0)) {
            return false; // 隣接していない
        }
        if (dr != 0 && dc != 0) {
            const int r0 = path[i - 1].first;
            const int c0 = path[i - 1].second;
            if (grid[r0 + dr][c0] != 0 || grid[r0][c0 + dc] != 0) {
                return false;
            }
        }
    }

    return true;
}

bool checkOptimalPath(const std::string& testName, const std::vector<std::vector<int>>& grid,
                       std::pair<int, int> start, std::pair<int, int> goal,
                       double expectedCost) {
    const auto path = jumpPointSearch(grid, start, goal);
    if (!isValidPath(grid, path, start, goal)) {
        std::cerr << "[FAIL] " << testName << ": invalid path returned" << std::endl;
        return false;
    }
    const double cost = pathCost(path);
    if (std::abs(cost - expectedCost) > kEps) {
        std::cerr << "[FAIL] " << testName << ": cost=" << cost << ", expected=" << expectedCost
                   << std::endl;
        return false;
    }
    std::cout << "[PASS] " << testName << std::endl;
    return true;
}

bool checkEmptyPath(const std::string& testName, const std::vector<std::vector<int>>& grid,
                     std::pair<int, int> start, std::pair<int, int> goal) {
    const auto path = jumpPointSearch(grid, start, goal);
    if (!path.empty()) {
        std::cerr << "[FAIL] " << testName << ": expected empty path but got non-empty"
                   << std::endl;
        return false;
    }
    std::cout << "[PASS] " << testName << std::endl;
    return true;
}

} // namespace

int main() {
    bool allPassed = true;

    {
        std::vector<std::vector<int>> grid(5, std::vector<int>(5, 0));
        allPassed &=
            checkOptimalPath("no obstacles diagonal path", grid, {0, 0}, {4, 4}, 4 * kSqrt2);
    }

    {
        std::vector<std::vector<int>> grid = {
            {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0},
        };
        allPassed &= checkOptimalPath("wall with gap detour", grid, {0, 0}, {4, 4}, 8.0);
    }

    {
        std::vector<std::vector<int>> grid(3, std::vector<int>(3, 0));
        allPassed &= checkOptimalPath("start equals goal", grid, {1, 1}, {1, 1}, 0.0);
    }

    {
        std::vector<std::vector<int>> grid = {
            {0, 0, 0, 0, 0},
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
        };
        allPassed &= checkEmptyPath("unreachable goal", grid, {0, 0}, {4, 4});
    }

    {
        std::vector<std::vector<int>> grid = {
            {0, 1, 0},
            {1, 0, 1},
            {0, 1, 0},
        };
        allPassed &= checkEmptyPath("corner cut blocked", grid, {0, 0}, {2, 2});
    }

    // 大きめのランダム障害物グリッドで、A*相当の最短コストとの整合性を
    // 単純な手計算ではなく「複数の障害物パターン」で検証する
    // （期待コスト13は独立実装のDijkstraで事前に計算・確認済み）。
    {
        std::vector<std::vector<int>> grid = {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0, 1, 1, 0, 1, 1, 1, 0},
            {0, 1, 0, 0, 0, 0, 1, 0},
            {0, 1, 0, 1, 1, 0, 1, 0},
            {0, 0, 0, 1, 0, 0, 0, 0},
            {0, 1, 1, 1, 0, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0},
        };
        allPassed &= checkOptimalPath("complex grid", grid, {0, 0}, {6, 7}, 13.0);
    }

    // コーナーカット禁止下では、近くに障害物が一つもない地点でも、最短経路が
    // 「強制されていない」斜めの折れを必要とすることがある（このリグレッション
    // テストが捉えている地点は(1,2)で、直進中に斜めへ折れないと
    // 最短コスト12+sqrt(2)を達成できない）。
    {
        std::vector<std::vector<int>> grid = {
            {0, 0, 1, 1, 1, 0},
            {0, 0, 0, 0, 0, 1},
            {0, 1, 0, 0, 0, 0},
            {1, 1, 1, 0, 0, 1},
            {0, 1, 0, 0, 1, 1},
            {0, 1, 0, 0, 0, 0},
            {1, 0, 1, 0, 1, 1},
            {0, 0, 1, 0, 0, 0},
            {0, 0, 0, 0, 0, 0},
            {0, 0, 1, 0, 1, 0},
        };
        allPassed &=
            checkOptimalPath("unforced diagonal shortcut", grid, {0, 1}, {8, 0}, 11.0 + kSqrt2);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
