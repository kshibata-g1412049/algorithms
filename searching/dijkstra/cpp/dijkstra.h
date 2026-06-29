#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <utility>
#include <vector>

// ダイクストラ法（Dijkstra's algorithm）
//
// グリッド上の単一始点最短経路を求めるアルゴリズム。スタート地点からの
// 実コスト g(n) が最小のノードから順に確定（=優先度付きキューから取り出して
// 展開）していくことで、各ノードへの最短コストを単調に確定させていく。
// ヒューリスティックを使わずに全方向へ均等にコストを伝播するため、
// 負の辺重みが無い限り常に最適解（最短経路）を返す。
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
std::vector<std::pair<int, int>> dijkstra(const std::vector<std::vector<int>>& grid,
                                           std::pair<int, int> start,
                                           std::pair<int, int> goal);

#endif // DIJKSTRA_H
