// weighted_astar.cppのテスト。
//
// このファイルは同じフォルダ内のweighted_astar.h/.cppのみに依存しており、
// 外部の共有ヘッダを使わない。そのためcpp/フォルダを単独で取り出しても
// `cmake .. && make && ctest` でビルド・実行できる。
//
// weighted_astarは最適性を保証しないため、検証内容は以下に限定する：
//   - 経路が見つかること
//   - 障害物を通らないこと
//   - 先頭がstart、末尾がgoalであること
//   - コストが理論上限（weight * 最適コスト）以下であること
#include "weighted_astar.h"

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
            return false;
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

bool checkBoundedPath(const std::string& testName, const std::vector<std::vector<int>>& grid,
                       std::pair<int, int> start, std::pair<int, int> goal, double weight,
                       double optimalCost) {
    const auto path = weightedAstar(grid, start, goal, weight);
    if (!isValidPath(grid, path, start, goal)) {
        std::cerr << "[FAIL] " << testName << ": invalid path returned" << std::endl;
        return false;
    }
    const double cost = pathCost(path);
    const double upperBound = weight * optimalCost + kEps;
    if (cost > upperBound) {
        std::cerr << "[FAIL] " << testName << ": cost=" << cost
                   << " exceeds upper bound=" << upperBound << std::endl;
        return false;
    }
    std::cout << "[PASS] " << testName << std::endl;
    return true;
}

bool checkEmptyPath(const std::string& testName, const std::vector<std::vector<int>>& grid,
                     std::pair<int, int> start, std::pair<int, int> goal, double weight) {
    const auto path = weightedAstar(grid, start, goal, weight);
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

    // 障害物なしの直線（斜め）経路。最適コストは4*sqrt(2)
    {
        std::vector<std::vector<int>> grid(5, std::vector<int>(5, 0));
        allPassed &= checkBoundedPath("no obstacles diagonal path", grid, {0, 0}, {4, 4}, 1.5,
                                       4 * kSqrt2);
    }

    // 壁に迂回が必要なケース。最適コストは8.0
    {
        std::vector<std::vector<int>> grid = {
            {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0},
        };
        allPassed &= checkBoundedPath("wall with gap detour", grid, {0, 0}, {4, 4}, 2.0, 8.0);
    }

    // weight=1.0（通常のA*と等価）でも最適コスト以下であることを確認
    {
        std::vector<std::vector<int>> grid = {
            {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0},
        };
        allPassed &= checkBoundedPath("weight equals one", grid, {0, 0}, {4, 4}, 1.0, 8.0);
    }

    // start == goal
    {
        std::vector<std::vector<int>> grid(3, std::vector<int>(3, 0));
        allPassed &= checkBoundedPath("start equals goal", grid, {1, 1}, {1, 1}, 2.0, 0.0);
    }

    // 到達不可能なケース（完全に壁で塞がれている）
    {
        std::vector<std::vector<int>> grid = {
            {0, 0, 0, 0, 0},
            {1, 1, 1, 1, 1},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
        };
        allPassed &= checkEmptyPath("unreachable goal", grid, {0, 0}, {4, 4}, 2.0);
    }

    // コーナーカット禁止の確認
    {
        std::vector<std::vector<int>> grid = {
            {0, 1, 0},
            {1, 0, 1},
            {0, 1, 0},
        };
        allPassed &= checkEmptyPath("corner cut blocked", grid, {0, 0}, {2, 2}, 2.0);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
