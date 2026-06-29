#ifndef TERNARY_SEARCH_H
#define TERNARY_SEARCH_H

#include <vector>

// 三分探索（Ternary Search）
//
// 探索範囲を3等分する2つの点mid1, mid2を取り、targetとの比較によって
// 3つの部分区間のうち探索を続けるべき1つに絞り込む。二分探索が範囲を
// 1/2に絞るのに対し、三分探索は1/3に絞り込むが、1回の反復で2回の比較を
// 行うため、比較回数の観点では二分探索より効率が劣ることが多い
// （定数倍の違い）。
//
// 前提条件: 入力配列arrは昇順にソート済みであること（事前条件の検証は行わない）。
//
// 時間複雑度: O(log3 n)
// 空間複雑度: O(1)（反復実装のため）
//
// arr: 昇順にソート済みの探索対象の配列
// target: 探したい値
// 戻り値: targetが見つかった場合はそのインデックス（0始まり。複数該当する
//         場合はいずれかのインデックス）、見つからなければ-1
int ternarySearch(const std::vector<int>& arr, int target);

#endif // TERNARY_SEARCH_H
