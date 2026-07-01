#ifndef WEIGHTED_ASTAR_H
#define WEIGHTED_ASTAR_H

#include <utility>
#include <vector>

// 重み付きA*探索（Weighted A* search）
//
// 通常のA*は f(n) = g(n) + h(n) でノードを評価するが、重み付きA*では
// f(n) = g(n) + weight * h(n) としてヒューリスティックの影響を強める。
// weight > 1.0 にするとヒューリスティックがより強く効くため、ゴール方向への
// 探索が加速し展開ノード数が減って高速になるが、ヒューリスティックが
// 許容的でなくなる（実コストを過大評価しうる）ため最適性は保証されない。
// weight = 1.0 のときは通常のA*と等価になる。
//
// 本実装ではヒューリスティックとして Octile distance を用いる。
//   dx = |r1 - r2|, dy = |c1 - c2|
//   h = max(dx, dy) + (sqrt(2) - 1) * min(dx, dy)
//
// 移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
// 斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
// その両側の直交マスのいずれかが障害物の場合は斜め移動できない。
//
// 時間複雑度: O(V log V + E)（V=セル数、E=辺数。実際にはweightが大きいほど
//             展開ノード数が減るため、通常のA*より高速になる傾向がある）
// 空間複雑度: O(V)
//
// grid:   2次元配列。0=通行可能, 1=障害物。
// start:  探索開始地点 (row, col)。
// goal:   目的地 (row, col)。
// weight: ヒューリスティックに掛ける重み（>1.0で高速化、最適性は保証されない）。
//
// 戻り値: startからgoalまでの経路を(row,col)のリストで返す
//         （start, goal両端を含む）。到達不可能な場合は空リストを返す。
//         経路の総コストはweight倍した理論上限（weight * 最適コスト）以下になる。
//
// 事前条件: start/goalはグリッド範囲内かつ通行可能マスであること
//           （呼び出し側の責務。本関数内では検証しない）。
std::vector<std::pair<int, int>> weightedAstar(const std::vector<std::vector<int>>& grid,
                                                std::pair<int, int> start,
                                                std::pair<int, int> goal, double weight);

#endif // WEIGHTED_ASTAR_H
