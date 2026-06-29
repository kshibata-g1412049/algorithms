#include "rrt_connect.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <random>

namespace {

// 木の1ノードを表す内部構造体。parentIndexで親ノードへのリンクを保持し、
// 経路復元時に木の任意のノードから根（start、もしくはgoal）までたどる。
struct Node {
    double x;
    double y;
    int parentIndex;  // 親ノードのインデックス。根ノードは-1。
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

// 木の中で、nodeIndexから親リンクをたどって根までの経路を復元する
// （根→nodeIndexの順ではなく、nodeIndex→根の順で返す。呼び出し側で
//  必要に応じて反転する）。
std::vector<std::pair<double, double>> pathToRoot(
    const std::vector<Node>& nodes, int nodeIndex) {
    std::vector<std::pair<double, double>> path;
    int current = nodeIndex;
    while (current != -1) {
        path.push_back({nodes[current].x, nodes[current].y});
        current = nodes[current].parentIndex;
    }
    return path;
}

// extend処理: treeに対して、サンプル点(sampleX, sampleY)方向へ1ステップだけ
// 伸ばす。最近傍ノードを探し、ステアし、衝突しなければ新しいノードをtreeに
// 追加する。追加した場合はtrueを返し、newIndexに新ノードのインデックスを
// 書き込む。衝突した場合はfalseを返す（基本RRTの1反復と同じ処理）。
bool extend(std::vector<Node>& tree, double sampleX, double sampleY,
            double stepSize,
            const std::vector<std::tuple<double, double, double>>& obstacles,
            int& newIndex) {
    const int nearestIndex = findNearestNodeIndex(tree, sampleX, sampleY);
    const Node& nearestNode = tree[nearestIndex];

    const auto newPoint =
        steer(nearestNode.x, nearestNode.y, sampleX, sampleY, stepSize);

    if (!isSegmentCollisionFree(nearestNode.x, nearestNode.y, newPoint.first,
                                 newPoint.second, obstacles)) {
        return false;
    }

    tree.push_back({newPoint.first, newPoint.second, nearestIndex});
    newIndex = static_cast<int>(tree.size()) - 1;
    return true;
}

// connect処理: treeの最近傍ノードから、targetX, targetYに向かって、
// 衝突しない限りstepSize刻みで繰り返しステアし続ける。targetに到達するか、
// 衝突するか、ステア回数の上限に達するまで継続する。
// 戻り値: 最終的にtreeに追加された最後のノードのインデックス
//        （1つも追加できなかった場合は-1）。
// reachedTarget: targetまでgoalRadius以内に到達できたかどうかを書き込む。
int connect(std::vector<Node>& tree, double targetX, double targetY,
            double stepSize, double goalRadius, int maxSteerSteps,
            const std::vector<std::tuple<double, double, double>>& obstacles,
            bool& reachedTarget) {
    reachedTarget = false;
    int lastAddedIndex = -1;

    for (int step = 0; step < maxSteerSteps; ++step) {
        int newIndex = -1;
        if (!extend(tree, targetX, targetY, stepSize, obstacles, newIndex)) {
            // 衝突して伸ばせなかった（trapped）。connectを中断する。
            break;
        }
        lastAddedIndex = newIndex;

        const double d = distance(tree[newIndex].x, tree[newIndex].y, targetX,
                                   targetY);
        if (d <= goalRadius) {
            reachedTarget = true;
            break;
        }
        if (d <= stepSize) {
            // 次のステアでtarget自体に到達する想定。もう一度だけ繰り返す。
            continue;
        }
    }

    return lastAddedIndex;
}

}  // namespace

std::vector<std::pair<double, double>> rrtConnect(
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

    // connectでのステア繰り返し回数の上限。探索空間の対角線長をstepSizeで
    // 割った値（往復しても十分な余裕を持たせるため2倍）を基準にする。
    const double diagonal = distance(xmin, ymin, xmax, ymax);
    const int maxSteerSteps =
        std::max(100, static_cast<int>(2.0 * diagonal / stepSize) + 10);

    // treeA = startを根とする木、treeB = goalを根とする木
    std::vector<Node> treeA;
    std::vector<Node> treeB;
    treeA.push_back({start.first, start.second, -1});
    treeB.push_back({goal.first, goal.second, -1});

    // swappedがtrueのとき、treeAが実際にはgoal側の木である
    // （経路結合時にどちらをstart側として扱うか判定するために使う）
    bool swapped = false;

    for (int iter = 0; iter < maxIterations; ++iter) {
        // 1. 探索空間内から完全にランダムな点をサンプリングする（goalバイアスなし）
        const double sampleX = distX(rng);
        const double sampleY = distY(rng);

        // 2. treeAに対してextend処理を行う（基本RRTの1ステップと同じ）
        int newIndexA = -1;
        if (extend(treeA, sampleX, sampleY, stepSize, obstacles, newIndexA)) {
            // 3. extendが成功したので、treeBから新しいノードへconnectを試みる
            const double targetX = treeA[newIndexA].x;
            const double targetY = treeA[newIndexA].y;

            bool reached = false;
            const int newIndexB =
                connect(treeB, targetX, targetY, stepSize, goalRadius,
                        maxSteerSteps, obstacles, reached);

            if (reached && newIndexB != -1) {
                // 4. 2つの木が接続できた。treeA側の経路とtreeB側の経路を結合する。
                // treeAの経路: root(treeA)→newIndexAの順に並べる
                // （pathToRootはnewIndexA→rootの順を返すので反転する）。
                std::vector<std::pair<double, double>> pathA =
                    pathToRoot(treeA, newIndexA);
                std::reverse(pathA.begin(), pathA.end());

                // treeBの経路: newIndexB→root(treeB)の順（そのままの順）。
                std::vector<std::pair<double, double>> pathB =
                    pathToRoot(treeB, newIndexB);
#ifdef RRT_CONNECT_DEBUG
                std::cerr << "DEBUG newIndexA=" << newIndexA << " (" << treeA[newIndexA].x << "," << treeA[newIndexA].y << ")"
                          << " newIndexB=" << newIndexB << " (" << treeB[newIndexB].x << "," << treeB[newIndexB].y << ")"
                          << " swapped=" << swapped << " pathA.size=" << pathA.size()
                          << " pathB.size=" << pathB.size() << std::endl;
                std::cerr << "pathB contents: ";
                for (auto&p : pathB) std::cerr << "(" << p.first << "," << p.second << ") ";
                std::cerr << std::endl;
#endif

                // treeAがstart側かgoal側かによって連結順を決める。
                std::vector<std::pair<double, double>> fullPath;
                if (!swapped) {
                    // treeA=start側、treeB=goal側
                    // pathA: start→接続点(treeA側) , pathB: 接続点(treeB側)→goal
                    fullPath = pathA;
                    fullPath.insert(fullPath.end(), pathB.begin(), pathB.end());
                } else {
                    // treeA=goal側、treeB=start側
                    // pathB: 接続点(treeB側)→start なので反転してstart→接続点に、
                    // pathA: 接続点(treeA側)→goal はそのまま末尾に連結する。
                    std::reverse(pathB.begin(), pathB.end());
                    fullPath = pathB;
                    fullPath.insert(fullPath.end(), pathA.begin(), pathA.end());
                }
                return fullPath;
            }
        }

        // 6. 次のイテレーションのためにtreeAとtreeBの役割を交換する
        std::swap(treeA, treeB);
        swapped = !swapped;
    }

    // max_iterations以内に接続できなかった
    return {};
}
