#ifndef EXPONENTIAL_SEARCH_H
#define EXPONENTIAL_SEARCH_H

#include <vector>

// 指数探索（Exponential Search）
//
// 探索範囲を1, 2, 4, 8, ... と倍々に広げていき、target以上の値を持つ
// 要素が現れた区間（あるいは配列の終端）を特定する。その後、特定した区間
// [prev, curr] に対して二分探索を行う。範囲が分かっている配列やtargetが
// 先頭に近い場合に有効で、特に上限が不明な（無限に続く）配列に対する
// 探索にも応用できる。
//
// 前提条件: 入力配列arrは昇順にソート済みであること（事前条件の検証は行わない）。
//
// 時間複雑度: O(log n)
// 空間複雑度: O(1)
//
// arr: 昇順にソート済みの探索対象の配列
// target: 探したい値
// 戻り値: targetが見つかった場合はそのインデックス（0始まり。複数該当する
//         場合はいずれかのインデックス）、見つからなければ-1
int exponentialSearch(const std::vector<int>& arr, int target);

#endif // EXPONENTIAL_SEARCH_H
