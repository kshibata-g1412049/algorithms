#ifndef ASTAR_H
#define ASTAR_H

#include <utility>
#include <vector>

// A*探索（A-star search）
//
// グリッド上の最短経路を求めるアルゴリズム。ダイクストラ法と同様に
// スタート地点からの実コスト g(n) を管理しつつ、さらにゴールまでの
// 推定コスト（ヒューリスティック）h(n) を加味した f(n) = g(n) + h(n) が
// 小さいノードから優先的に展開する。h(n) が許容的（admissible：実際の
// 最短コストを過大評価しない）であれば、A*は常に最適解を返す。
//
// 本実装ではヒューリスティックとして Octile distance を用いる。
//   dx = |r1 - r2|, dy = |c1 - c2|
//   h = max(dx, dy) + (sqrt(2) - 1) * min(dx, dy)
// これは8方向移動（直進コスト1、斜め移動コストsqrt(2)）に対して
// 許容的なヒューリスティックである。
//
// 移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
// 斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
// その両側の直交マスのいずれかが障害物の場合は斜め移動できない。
//
// 時間複雑度: O(V log V + E)（V=セル数、E=辺数。二分ヒープによる
//             優先度付きキューを使用した場合）
// 空間複雑度: O(V)（距離・親情報・優先度付きキューの保持に必要）
//
// grid:  2次元配列。0=通行可能, 1=障害物。
// start: 探索開始地点 (row, col)。
// goal:  目的地 (row, col)。
//
// 戻り値: startからgoalまでの最短経路を(row,col)のリストで返す
//         （start, goal両端を含む）。到達不可能な場合は空リストを返す。
//
// 事前条件: start/goalはグリッド範囲内かつ通行可能マスであること
//           （呼び出し側の責務。本関数内では検証しない）。
std::vector<std::pair<int, int>> astar(const std::vector<std::vector<int>>& grid,
                                        std::pair<int, int> start,
                                        std::pair<int, int> goal);

#endif // ASTAR_H
