#include "goal_biased_rrt.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <random>

namespace {

// goalをサンプル点として選ぶ確率（ゴールバイアス）。残り(1 - kGoalBiasProbability)
// の確率では探索空間内に完全ランダムな点をサンプリングする。
constexpr double kGoalBiasProbability = 0.1;

// 木の1ノードを表す内部構造体。parentIndexで親ノードへのリンクを保持し、
// 経路復元時にgoal付近のノードから根（start）までたどる。
struct Node {
    double x;
    double y;
    int parentIndex;  // 親ノードのインデックス。startノードは-1。
};

// 点(px, py)と線分(x1,y1)-(x2,y2)との最短距離を求める。
double pointToSegmentDistance(double px, double py, double x1, double y1,
                               double x2, double y2) {
    const double dx = x2 - x1;
    const double dy = y2 - y1;
    const double lengthSquared = dx * dx + dy * dy;

    if (lengthSquared < 1e-12) {
        // 線分の長さがほぼ0 = 1点とみなして点間距離を返す
        const double ddx = px - x1;
        const double ddy = py - y1;
        return std::sqrt(ddx * ddx + ddy * ddy);
    }

    // 線分上の最近点を表すパラメータt（[0,1]にクランプ）を求める
    double t = ((px - x1) * dx + (py - y1) * dy) / lengthSquared;
    t = std::max(0.0, std::min(1.0, t));

    const double closestX = x1 + t * dx;
    const double closestY = y1 + t * dy;
    const double ddx = px - closestX;
    const double ddy = py - closestY;
    return std::sqrt(ddx * ddx + ddy * ddy);
}

// 線分(x1,y1)-(x2,y2)が円(cx,cy,radius)と衝突しているかどうかを判定する。
// 点と線分の最短距離がradius以下であれば衝突とみなす。
bool segmentIntersectsCircle(double x1, double y1, double x2, double y2,
                              double cx, double cy, double radius) {
    return pointToSegmentDistance(cx, cy, x1, y1, x2, y2) <= radius;
}

// 線分(x1,y1)-(x2,y2)が、与えられた全ての障害物のいずれとも衝突しないかを判定する。
bool isSegmentCollisionFree(
    double x1, double y1, double x2, double y2,
    const std::vector<std::tuple<double, double, double>>& obstacles) {
    for (const auto& obstacle : obstacles) {
        const double cx = std::get<0>(obstacle);
        const double cy = std::get<1>(obstacle);
        const double radius = std::get<2>(obstacle);
        if (segmentIntersectsCircle(x1, y1, x2, y2, cx, cy, radius)) {
            return false;
        }
    }
    return true;
}

// 2点間のユークリッド距離。
double distance(double x1, double y1, double x2, double y2) {
    const double dx = x2 - x1;
    const double dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

// fromノードからtoward方向にstepSize分だけ進んだ新しい点を計算する
// （toward自体がstepSize以内にある場合はtoward自体を返す）。
std::pair<double, double> steer(double fromX, double fromY, double towardX,
                                 double towardY, double stepSize) {
    const double dist = distance(fromX, fromY, towardX, towardY);
    if (dist <= stepSize) {
        return {towardX, towardY};
    }
    const double ratio = stepSize / dist;
    return {fromX + (towardX - fromX) * ratio, fromY + (towardY - fromY) * ratio};
}

// 木の中でサンプル点(x, y)に最も近いノードのインデックスを線形探索で求める。
// 学習用途のためナイーブなO(n)実装。本格的な実装ではKD-tree等で高速化する。
int findNearestNodeIndex(const std::vector<Node>& nodes, double x, double y) {
    int nearestIndex = 0;
    double nearestDistance = std::numeric_limits<double>::max();
    for (size_t i = 0; i < nodes.size(); ++i) {
        const double d = distance(nodes[i].x, nodes[i].y, x, y);
        if (d < nearestDistance) {
            nearestDistance = d;
            nearestIndex = static_cast<int>(i);
        }
    }
    return nearestIndex;
}

// 木の中で、goalノードからnodeIndexまで親リンクをたどって経路を復元する。
std::vector<std::pair<double, double>> reconstructPath(
    const std::vector<Node>& nodes, int nodeIndex) {
    std::vector<std::pair<double, double>> path;
    int current = nodeIndex;
    while (current != -1) {
        path.push_back({nodes[current].x, nodes[current].y});
        current = nodes[current].parentIndex;
    }
    // 復元順はgoal->startなので、start->goalの順に反転する
    std::reverse(path.begin(), path.end());
    return path;
}

}  // namespace

std::vector<std::pair<double, double>> goalBiasedRrt(
    const std::vector<std::tuple<double, double, double>>& obstacles,
    std::array<double, 4> bounds, std::pair<double, double> start,
    std::pair<double, double> goal, double goalRadius, double stepSize,
    int maxIterations, unsigned int seed) {
    const double xmin = bounds[0];
    const double ymin = bounds[1];
    const double xmax = bounds[2];
    const double ymax = bounds[3];

    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> distX(xmin, xmax);
    std::uniform_real_distribution<double> distY(ymin, ymax);
    std::uniform_real_distribution<double> distBias(0.0, 1.0);

    std::vector<Node> nodes;
    nodes.push_back({start.first, start.second, -1});

    for (int iter = 0; iter < maxIterations; ++iter) {
        // 1. kGoalBiasProbabilityの確率でgoal自体をサンプル点として使う
        //    （ゴールバイアス）。それ以外は探索空間内に完全ランダムな点を
        //    サンプリングする。
        double sampleX;
        double sampleY;
        if (distBias(rng) < kGoalBiasProbability) {
            sampleX = goal.first;
            sampleY = goal.second;
        } else {
            sampleX = distX(rng);
            sampleY = distY(rng);
        }

        // 2. 木の中でサンプル点に最も近いノードを探す（線形探索、O(n)）
        const int nearestIndex = findNearestNodeIndex(nodes, sampleX, sampleY);
        const Node& nearestNode = nodes[nearestIndex];

        // 3. 最近傍ノードからサンプル点方向にstepSize分だけ伸ばした新しい点を作る
        const auto newPoint =
            steer(nearestNode.x, nearestNode.y, sampleX, sampleY, stepSize);

        // 4. 新しい点への線分が障害物と衝突していなければ木に追加する
        if (!isSegmentCollisionFree(nearestNode.x, nearestNode.y, newPoint.first,
                                     newPoint.second, obstacles)) {
            continue;
        }

        nodes.push_back({newPoint.first, newPoint.second, nearestIndex});
        const int newIndex = static_cast<int>(nodes.size()) - 1;

        // 5. 新しいノードがgoalからgoalRadius以内であれば経路を復元して返す
        if (distance(newPoint.first, newPoint.second, goal.first, goal.second) <=
            goalRadius) {
            return reconstructPath(nodes, newIndex);
        }
    }

    // maxIterations以内にgoalへ到達できなかった
    return {};
}
