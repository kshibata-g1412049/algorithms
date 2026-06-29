#ifndef GOAL_BIASED_RRT_H
#define GOAL_BIASED_RRT_H

#include <array>
#include <tuple>
#include <utility>
#include <vector>

// Goal-biased RRT（ゴールバイアス付き高速探索ランダム木）
//
// 基本のRRTと同様に、2次元連続空間上でstartを根とする木をランダムサンプリング
// によって段階的に広げていき、goal付近まで到達する経路を探索するサンプリング
// ベースの経路計画アルゴリズム。各反復で以下を行う:
//   1. 乱数を1つ生成し、それがkGoalBiasProbability未満であればサンプル点として
//      goal座標そのものを使う（ゴールバイアス）。それ以外の場合
//      （1 - kGoalBiasProbability の確率）は探索空間内に完全ランダムな点を
//      サンプリングする。これが基本のRRTとの唯一の違いである。
//   2. 木の中でサンプル点に最も近いノード（最近傍ノード）を見つける。
//   3. 最近傍ノードからサンプル点方向にstep_size分だけ進んだ新しいノードを作る
//      （これを「ステア」と呼ぶ）。
//   4. 最近傍ノードから新しいノードへの線分が障害物と衝突しなければ、
//      新しいノードを木に追加する。
//   5. 新しいノードがgoalからgoal_radius以内であれば探索を終了し、
//      startから新しいノードまでの経路を木の親リンクをたどって復元する。
//
// 一定確率でgoal自体をサンプル点として使うことで、木がgoal方向へ伸びやすくなり、
// 障害物が少ない/単純なシナリオでは基本のRRTより少ない反復回数で収束する傾向が
// ある（ただしgoalバイアスが強すぎると局所的な障害物にトラップされやすくなる
// トレードオフがあるため、確率は小さめの値、例えば10%程度に設定する）。
//
// 時間・空間複雑度:
//   最近傍探索は、学習用途のため木の全ノードを線形走査するナイーブな実装
//   （O(n)、nは現在の木のノード数）になっている。本格的な実装ではKD-tree等の
//   空間分割構造を使って最近傍探索をO(log n)程度に高速化する。
//   1イテレーションあたりO(n)の最近傍探索を行うため、合計でO(n^2)程度
//   （n = max_iterations、最悪ケース）。空間複雑度はノードを保持するためO(n)。
//
// obstacles: 障害物のリスト。各要素は(center_x, center_y, radius)のタプル。
// bounds: 探索空間の矩形領域 {xmin, ymin, xmax, ymax}。
// start: 探索開始点(x, y)。
// goal: 目標点(x, y)。
// goalRadius: goalからこの半径以内に到達したら成功とみなす。
// stepSize: 1ステップで木を伸ばす距離。
// maxIterations: 最大反復回数。これを超えても到達できなければ空の経路を返す。
// seed: 乱数生成器（std::mt19937）を初期化するシード値（再現性確保のため必須）。
//
// 戻り値: startからgoal付近までの経路（(x,y)の点のリスト）。
//         先頭がstart、末尾がgoal付近の到達点。見つからなければ空のvector。
std::vector<std::pair<double, double>> goalBiasedRrt(
    const std::vector<std::tuple<double, double, double>>& obstacles,
    std::array<double, 4> bounds, std::pair<double, double> start,
    std::pair<double, double> goal, double goalRadius, double stepSize,
    int maxIterations, unsigned int seed);

#endif  // GOAL_BIASED_RRT_H
