// bidirectional_astar.cppのテスト。
//
// このファイルは同じフォルダ内のbidirectional_astar.h/.cppのみに依存しており、
// 外部の共有ヘッダを使わない。そのためcpp/フォルダを単独で取り出しても
// `cmake .. && make && ctest` でビルド・実行できる。
#include "bidirectional_astar.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>

namespace {

constexpr double kEps = 1e-6;
constexpr double kSqrt2 = 1.4142135623730951;

// 経路の総コスト（直進1・斜めsqrt(2)の合計）を計算する。
double pathCost(const std::vector<std::pair<int, int>>& path) {
    double cost = 0.0;
    for (size_t i = 1; i < path.size(); ++i) {
        const int dr = std::abs(path[i].first - path[i - 1].first);
        const int dc = std::abs(path[i].second - path[i - 1].second);
        cost += (dr == 1 && dc == 1) ? kSqrt2 : 1.0;
    }
    return cost;
}

// 経路が以下の制約を満たすかを検証する:
//  - 先頭がstart、末尾がgoal
//  - 各ステップが8方向のいずれかで隣接している
//  - 障害物セルを通らない
//  - 斜め移動時にコーナーカットしていない
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
            // 斜め移動: コーナーカット禁止を確認
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
    const auto path = bidirectionalAstar(grid, start, goal);
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
    const auto path = bidirectionalAstar(grid, start, goal);
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

    // 障害物なしの直線（斜め）経路
    {
        std::vector<std::vector<int>> grid(5, std::vector<int>(5, 0));
        allPassed &=
            checkOptimalPath("no obstacles diagonal path", grid, {0, 0}, {4, 4}, 4 * kSqrt2);
    }

    // 壁に迂回が必要なケース
    {
        std::vector<std::vector<int>> grid = {
            {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0}, {0, 1, 1, 1, 0}, {0, 0, 0, 0, 0},
        };
        allPassed &= checkOptimalPath("wall with gap detour", grid, {0, 0}, {4, 4}, 8.0);
    }

    // start == goal
    {
        std::vector<std::vector<int>> grid(3, std::vector<int>(3, 0));
        allPassed &= checkOptimalPath("start equals goal", grid, {1, 1}, {1, 1}, 0.0);
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
        allPassed &= checkEmptyPath("unreachable goal", grid, {0, 0}, {4, 4});
    }

    // コーナーカット禁止の確認（対角の隙間を斜め移動で突き抜けないこと）
    {
        std::vector<std::vector<int>> grid = {
            {0, 1, 0},
            {1, 0, 1},
            {0, 1, 0},
        };
        // (0,0)から(2,2)へは中央(1,1)を経由する必要があるが、
        // (0,0)->(1,1)の斜め移動は両側(0,1),(1,0)が障害物なので禁止される。
        allPassed &= checkEmptyPath("corner cut blocked", grid, {0, 0}, {2, 2});
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
