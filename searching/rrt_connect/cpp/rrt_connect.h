#ifndef RRT_CONNECT_H
#define RRT_CONNECT_H

#include <array>
#include <tuple>
#include <utility>
#include <vector>

// RRT-Connect（双方向RRT / Bidirectional Rapidly-exploring Random Tree）
//
// startを根とする木（treeA）とgoalを根とする木（treeB）の2本を同時に育て、
// 互いに向かって伸ばし合うことで通常のRRTより高速にstart-goal間の経路を見つける
// サンプリングベースの経路計画アルゴリズム。各反復で以下を行う:
//   1. 探索空間内からランダムに点（サンプル点）を1つ選ぶ（goalバイアスなし）。
//   2. treeAに対して「extend」処理を行う: treeA内の最近傍ノードを見つけ、
//      サンプル点方向にstep_size分ステアし、衝突しなければ新しいノードを
//      treeAに追加する（基本RRTの1ステップと同じ処理）。
//   3. extendが成功したら、その新しいノードに向けてtreeBから「connect」を
//      試みる: treeB内の最近傍ノードから新しいノードに向かって、衝突しない
//      限りstep_size刻みで繰り返しステアし続ける（targetに到達するか、
//      衝突するか、ステア回数の上限に達するまで）。これによりtreeBがtreeAの
//      新しいノードに「届く」可能性がある。
//   4. 2つの木のノードがgoal_radius以内（またはstep_size以内）まで近づいたら
//      接続成功とみなし、treeA側の経路（root→接続点）とtreeB側の経路
//      （接続点→root）を結合してstart→goalの完全な経路を構築する。
//      treeBはgoalを根にしているため、treeBの経路は反転させてから連結する。
//   5. 次のイテレーションではtreeAとtreeBの役割を交換し、両方の木が均等に
//      成長するようにする。
//   6. max_iterations以内に接続できなければ空の経路を返す。
//
// 厳密なRRT-Connect論文のtrapped/advanced状態遷移は再現していない簡略版だが、
// 「両方の木が交互に伸び、互いに接続を試み、接続できたら経路を結合する」という
// 核心的な振る舞いは実装している。connectのステア繰り返し回数には上限を設けて
// 無限ループを避けている。
//
// 時間・空間複雑度:
//   最近傍探索は、学習用途のため木の全ノードを線形走査するナイーブな実装
//   （O(n)、nは木のノード数）になっている。本格的な実装ではKD-tree等の
//   空間分割構造を使って最近傍探索をO(log n)程度に高速化する。
//   1イテレーションではextendで最近傍探索1回（O(n)）、connectで複数回の
//   ステア・最近傍探索（定数倍はconnectの繰り返し回数の上限）を行うため
//   1イテレーションあたりO(n)程度。合計でO(n^2)程度
//   （n = max_iterations、最悪ケース）。空間複雑度は2本の木のノードを
//   保持するためO(n)。
//
// obstacles: 障害物のリスト。各要素は(center_x, center_y, radius)のタプル。
// bounds: 探索空間の矩形領域 {xmin, ymin, xmax, ymax}。
// start: 探索開始点(x, y)。
// goal: 目標点(x, y)。
// goalRadius: 2つの木のノードがこの半径以内まで近づいたら接続成功とみなす。
// stepSize: 1ステップで木を伸ばす距離。
// maxIterations: 最大反復回数。これを超えても接続できなければ空の経路を返す。
// seed: 乱数生成器（std::mt19937）を初期化するシード値（再現性確保のため必須）。
//
// 戻り値: startからgoal付近までの経路（(x,y)の点のリスト）。
//         先頭がstart、末尾がgoal付近の到達点。見つからなければ空のvector。
std::vector<std::pair<double, double>> rrtConnect(
    const std::vector<std::tuple<double, double, double>>& obstacles,
    std::array<double, 4> bounds, std::pair<double, double> start,
    std::pair<double, double> goal, double goalRadius, double stepSize,
    int maxIterations, unsigned int seed);

#endif  // RRT_CONNECT_H
