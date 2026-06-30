// informed_rrt_star.cppのテスト。
//
// このファイルは同じフォルダ内のinformed_rrt_star.h/.cppのみに依存しており、
// 外部の共有ヘッダを使わない。そのためcpp/フォルダを単独で取り出しても
// `cmake .. && make && ctest` でビルド・実行できる。
//
// Informed RRT*は乱数を使うアルゴリズムであり、固定シードでもC++/Python/MATLAB
// 間で同じ乱数列にはならない（言語ごとに乱数生成器の実装が異なるため）。
// また本実装はmaxIterations回まで探索を継続し、見つかった中で最良コストの
// 経路を返す設計であり、厳密な最適性（コストが本当に改善されること）の検証は
// ここでは行わない（flakyになるため）。そのため、ここでは出力の厳密な値ではなく、
// 以下の「性質」を検証するプロパティベーステストを行う:
//   1. 十分大きなmaxIterationsであれば、複数の固定シードで経路が見つかる。
//   2. 経路の先頭がstartと一致する。
//   3. 経路の末尾がgoalからgoalRadius以内にある。
//   4. 経路上の各線分が障害物と衝突していない。
//   5. 経路上の各ステップの移動距離がstepSize以下（許容誤差込み）である。
//   6. 障害物による迂回が必要なシナリオでも1〜5が成り立つ。
//   7. maxIterationsが極端に小さい場合は空、または性質を満たす経路のみが返る。
#include "informed_rrt_star.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <vector>

namespace {

double distance(double x1, double y1, double x2, double y2) {
    const double dx = x2 - x1;
    const double dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

// 点と線分の最短距離（テスト側でも衝突判定を独立に再実装して検証する）。
double pointToSegmentDistance(double px, double py, double x1, double y1,
                               double x2, double y2) {
    const double dx = x2 - x1;
    const double dy = y2 - y1;
    const double lengthSquared = dx * dx + dy * dy;
    if (lengthSquared < 1e-12) {
        return distance(px, py, x1, y1);
    }
    double t = ((px - x1) * dx + (py - y1) * dy) / lengthSquared;
    t = std::max(0.0, std::min(1.0, t));
    const double closestX = x1 + t * dx;
    const double closestY = y1 + t * dy;
    return distance(px, py, closestX, closestY);
}

bool checkPathProperties(
    const std::string& testName,
    const std::vector<std::pair<double, double>>& path,
    std::pair<double, double> start, std::pair<double, double> goal,
    double goalRadius, double stepSize,
    const std::vector<std::tuple<double, double, double>>& obstacles,
    bool expectNonEmpty) {
    if (path.empty()) {
        if (expectNonEmpty) {
            std::cerr << "[FAIL] " << testName << ": path is empty but expected non-empty"
                      << std::endl;
            return false;
        }
        std::cout << "[PASS] " << testName << " (empty path, allowed)" << std::endl;
        return true;
    }

    // 2. 先頭がstartと一致する
    if (std::abs(path.front().first - start.first) > 1e-9 ||
        std::abs(path.front().second - start.second) > 1e-9) {
        std::cerr << "[FAIL] " << testName << ": path does not start at start" << std::endl;
        return false;
    }

    // 3. 末尾がgoalからgoalRadius以内にある（浮動小数点許容誤差込み）
    const double finalDistance =
        distance(path.back().first, path.back().second, goal.first, goal.second);
    if (finalDistance > goalRadius + 1e-6) {
        std::cerr << "[FAIL] " << testName << ": final point distance to goal = "
                  << finalDistance << " > goalRadius = " << goalRadius << std::endl;
        return false;
    }

    // 4. 経路上の各線分が障害物と衝突していない
    // 5. 各ステップの移動距離がstepSize以下（許容誤差込み）
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        const double stepDist = distance(path[i].first, path[i].second,
                                          path[i + 1].first, path[i + 1].second);
        if (stepDist > stepSize * 1.01) {
            std::cerr << "[FAIL] " << testName << ": step " << i
                      << " distance = " << stepDist << " exceeds stepSize*1.01 = "
                      << stepSize * 1.01 << std::endl;
            return false;
        }

        for (const auto& obstacle : obstacles) {
            const double cx = std::get<0>(obstacle);
            const double cy = std::get<1>(obstacle);
            const double radius = std::get<2>(obstacle);
            const double d = pointToSegmentDistance(cx, cy, path[i].first, path[i].second,
                                                      path[i + 1].first, path[i + 1].second);
            if (d <= radius) {
                std::cerr << "[FAIL] " << testName << ": segment " << i
                          << " collides with obstacle at (" << cx << ", " << cy << ")"
                          << std::endl;
                return false;
            }
        }
    }

    std::cout << "[PASS] " << testName << " (path length=" << path.size() << ")"
              << std::endl;
    return true;
}

}  // namespace

int main() {
    bool allPassed = true;

    const std::array<double, 4> bounds = {0.0, 0.0, 10.0, 10.0};
    const std::pair<double, double> start = {1.0, 1.0};
    const std::pair<double, double> goal = {9.0, 9.0};
    const double goalRadius = 0.5;
    const double stepSize = 0.5;
    // Informed RRT*はRRT/RRT*と異なり最初の解で打ち切らずmaxIterations回
    // 必ず最後まで実行する（O(n^2)）ため、テストの実行時間を抑えるために
    // RRT/RRT*テストより小さいmaxIterationsを使う（経験上2000で両シナリオとも
    // 複数シードで安定して経路が見つかる）。
    const int maxIterations = 2000;

    // 1, 2, 3, 4, 5: 障害物なしのシンプルなシナリオを複数シードで検証
    {
        std::vector<std::tuple<double, double, double>> obstacles;
        for (unsigned int seed : {1u, 2u, 3u}) {
            const auto path = informedRrtStar(obstacles, bounds, start, goal, goalRadius,
                                               stepSize, maxIterations, seed);
            allPassed &= checkPathProperties("no obstacles, seed=" + std::to_string(seed),
                                              path, start, goal, goalRadius, stepSize,
                                              obstacles, /*expectNonEmpty=*/true);
        }
    }

    // 6: 障害物があり迂回が必要なシナリオ（start-goal間に大きな円障害物を配置）
    {
        std::vector<std::tuple<double, double, double>> obstacles = {
            std::make_tuple(5.0, 5.0, 2.0)};
        for (unsigned int seed : {1u, 2u, 3u}) {
            const auto path = informedRrtStar(obstacles, bounds, start, goal, goalRadius,
                                               stepSize, maxIterations, seed);
            allPassed &= checkPathProperties(
                "with obstacle, seed=" + std::to_string(seed), path, start, goal,
                goalRadius, stepSize, obstacles, /*expectNonEmpty=*/true);
        }
    }

    // 7: maxIterationsが極端に小さい場合（空、または性質を満たす経路のみ許容）
    {
        std::vector<std::tuple<double, double, double>> obstacles;
        const auto path =
            informedRrtStar(obstacles, bounds, start, goal, goalRadius, stepSize, 1, 42u);
        allPassed &= checkPathProperties("tiny max_iterations", path, start, goal,
                                          goalRadius, stepSize, obstacles,
                                          /*expectNonEmpty=*/false);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
