#include "rrt_star.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include <vector>

namespace {

// 木の1ノードを表す内部構造体。parentIndexで親ノードへのリンクを保持し、
// costはstartからこのノードまでの累積距離（経路長）を保持する。
struct Node {
    double x;
    double y;
    int parentIndex;  // 親ノードのインデックス。startノードは-1。
    double cost;       // startからこのノードまでの累積コスト（距離）。
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

// 点(x, y)からneighborRadius以内にある全ノードのインデックスを線形探索で求める
// （親選択とリワイヤで共通して使う近傍集合）。学習用途のためO(n)実装。
std::vector<int> findNeighborIndices(const std::vector<Node>& nodes, double x,
                                      double y, double neighborRadius) {
    std::vector<int> neighbors;
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (distance(nodes[i].x, nodes[i].y, x, y) <= neighborRadius) {
            neighbors.push_back(static_cast<int>(i));
        }
    }
    return neighbors;
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

// parentIndexがnodeIndexの子ノードのコストを再帰的に更新する
// （リワイヤによって親ノードのコストが変わったとき、子孫全体に伝播させるため）。
// 学習用途のため、親リンクをたどる代わりに毎回全ノードを線形走査して
// 直接の子を探す素朴な実装にしている（O(n)、深い木ではさらに再帰呼び出し分かかる）。
void updateDescendantCosts(std::vector<Node>& nodes, int nodeIndex) {
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (nodes[i].parentIndex == nodeIndex) {
            nodes[i].cost = nodes[nodeIndex].cost +
                             distance(nodes[nodeIndex].x, nodes[nodeIndex].y,
                                      nodes[i].x, nodes[i].y);
            updateDescendantCosts(nodes, static_cast<int>(i));
        }
    }
}

}  // namespace

std::vector<std::pair<double, double>> rrtStar(
    const std::vector<std::tuple<double, double, double>>& obstacles,
    std::array<double, 4> bounds, std::pair<double, double> start,
    std::pair<double, double> goal, double goalRadius, double stepSize,
    int maxIterations, unsigned int seed) {
    const double xmin = bounds[0];
    const double ymin = bounds[1];
    const double xmax = bounds[2];
    const double ymax = bounds[3];

    // 親選択・リワイヤで使う近傍半径。シンプルな固定値（stepSizeの定数倍）を
    // 採用する。値が大きいほど最適化の効果は高まるが、近傍探索・リワイヤの
    // コストも増える。
    const double neighborRadius = stepSize * 5.0;

    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> distX(xmin, xmax);
    std::uniform_real_distribution<double> distY(ymin, ymax);

    std::vector<Node> nodes;
    nodes.push_back({start.first, start.second, -1, 0.0});

    for (int iter = 0; iter < maxIterations; ++iter) {
        // 1. 探索空間内から完全にランダムな点をサンプリングする（goalバイアスなし）
        const double sampleX = distX(rng);
        const double sampleY = distY(rng);

        // 2. 木の中でサンプル点に最も近いノードを探す（線形探索、O(n)）
        const int nearestIndex = findNearestNodeIndex(nodes, sampleX, sampleY);
        const Node& nearestNode = nodes[nearestIndex];

        // 3. 最近傍ノードからサンプル点方向にstepSize分だけ伸ばした新しい点を作る
        const auto newPoint =
            steer(nearestNode.x, nearestNode.y, sampleX, sampleY, stepSize);

        // 4. 新しい点への線分が障害物と衝突していなければ次に進む
        if (!isSegmentCollisionFree(nearestNode.x, nearestNode.y, newPoint.first,
                                     newPoint.second, obstacles)) {
            continue;
        }

        // 5. 親選択（choose parent）: 新しい点の近傍集合の中から、
        //    「ノードのコスト + そのノードから新しい点までの距離」が最小で、
        //    かつ間の線分が衝突しないノードを親として選ぶ。
        //    近傍集合自体はneighborRadius（stepSizeより広い範囲）で探すが、
        //    実際に親候補・リワイヤ対象として採用するのはstepSize以内の
        //    ノードに限定する。これにより、木のどの辺も長さがstepSize以下に
        //    保たれ、基本RRTと同様に「1ステップの移動距離はstepSize以下」
        //    という性質を維持できる（経路の検証や可視化を単純にするための
        //    学習用途の簡略化。本来のRRT*はこの制約を置かず、近傍内であれば
        //    距離に関わらず辺を張ってよい）。
        const std::vector<int> neighbors =
            findNeighborIndices(nodes, newPoint.first, newPoint.second, neighborRadius);

        int bestParentIndex = nearestIndex;
        double bestCost =
            nearestNode.cost + distance(nearestNode.x, nearestNode.y, newPoint.first,
                                         newPoint.second);

        for (int candidateIndex : neighbors) {
            const Node& candidate = nodes[candidateIndex];
            const double candidateDist =
                distance(candidate.x, candidate.y, newPoint.first, newPoint.second);
            if (candidateDist > stepSize) {
                continue;
            }
            const double candidateCost = candidate.cost + candidateDist;
            if (candidateCost < bestCost &&
                isSegmentCollisionFree(candidate.x, candidate.y, newPoint.first,
                                       newPoint.second, obstacles)) {
                bestCost = candidateCost;
                bestParentIndex = candidateIndex;
            }
        }

        // 6. 新しい点をそのコストとともに木に追加する。
        nodes.push_back({newPoint.first, newPoint.second, bestParentIndex, bestCost});
        const int newIndex = static_cast<int>(nodes.size()) - 1;

        // 7. リワイヤ（rewire）: 新しいノードの近傍集合にある既存の各ノードについて、
        //    新しいノード経由のコストが現在のコストより小さく、かつ衝突しないなら
        //    親を新しいノードに付け替え、コストを更新する（子孫のコストも伝播）。
        //    こちらもstepSize以内のノードのみを対象にする（上記と同じ理由）。
        for (int neighborIndex : neighbors) {
            if (neighborIndex == bestParentIndex) {
                continue;
            }
            Node& neighbor = nodes[neighborIndex];
            const double distToNeighbor =
                distance(newPoint.first, newPoint.second, neighbor.x, neighbor.y);
            if (distToNeighbor > stepSize) {
                continue;
            }
            const double costThroughNew = nodes[newIndex].cost + distToNeighbor;
            if (costThroughNew < neighbor.cost &&
                isSegmentCollisionFree(newPoint.first, newPoint.second, neighbor.x,
                                       neighbor.y, obstacles)) {
                neighbor.parentIndex = newIndex;
                neighbor.cost = costThroughNew;
                updateDescendantCosts(nodes, neighborIndex);
            }
        }

        // 8. 新しいノードがgoalからgoalRadius以内であれば、その時点で経路を
        //    復元して返す。学習用途のため、本実装ではgoal到達時点で即座に
        //    経路を返す。より高度な実装では全イテレーション後に
        //    最良コストの経路を選ぶ。
        if (distance(newPoint.first, newPoint.second, goal.first, goal.second) <=
            goalRadius) {
            return reconstructPath(nodes, newIndex);
        }
    }

    // maxIterations以内にgoalへ到達できなかった
    return {};
}
