#ifndef BIDIRECTIONAL_ASTAR_H
#define BIDIRECTIONAL_ASTAR_H

#include <utility>
#include <vector>

// 双方向A*探索（Bidirectional A* search）
//
// startからgoalへの通常のA*探索（前進探索）と、goalからstartへの
// A*探索（後退探索）を同時に走らせ、2つの探索の確定済み（closed）集合が
// 共通のノードを持った時点で、両方向の最短経路を結合して全体の最短経路を
// 構築するアルゴリズム。各方向のヒューリスティックにはOctile distanceを
// 使用し、前進探索は相手側のgoal、後退探索は相手側のstartを目標とする。
//
// 終了条件: 両側の優先度付きキューを交互に1ステップずつ展開し、ある
// ノードが両方のclosed集合に含まれた瞬間、それを「交差点(meeting node)」
// とみなす。その時点で両側の優先度付きキューに残る最小f値の合計が
// これまでに見つかった最良の合計コストを超えないことを確認してから
// 探索を終了することで、最適解を保証する（標準的な双方向探索の停止条件）。
//
// 移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
// 斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
// その両側の直交マスのいずれかが障害物の場合は斜め移動できない。
//
// 時間複雑度: O(V log V + E)（V=セル数、E=辺数。理論上は単方向A*よりも
//             探索範囲が小さくなることが多く実用上高速になる傾向がある）
// 空間複雑度: O(V)（両方向の探索状態を保持するため定数倍は大きくなる）
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
std::vector<std::pair<int, int>> bidirectionalAstar(const std::vector<std::vector<int>>& grid,
                                                      std::pair<int, int> start,
                                                      std::pair<int, int> goal);

#endif // BIDIRECTIONAL_ASTAR_H
