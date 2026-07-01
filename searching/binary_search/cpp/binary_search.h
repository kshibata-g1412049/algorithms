#ifndef BINARY_SEARCH_H
#define BINARY_SEARCH_H

#include <vector>

// 二分探索（Binary Search）
//
// 探索範囲の中央要素とtargetを比較し、targetが中央要素より小さければ
// 左半分、大きければ右半分に絞り込むことを繰り返す。比較を1回行うたびに
// 探索範囲が半分になるため、非常に高速に探索できる。
//
// 前提条件: 入力配列arrは昇順にソート済みであること（事前条件の検証は行わない）。
//
// 時間複雑度: O(log n)
// 空間複雑度: O(1)（反復実装のため）
//
// arr: 昇順にソート済みの探索対象の配列
// target: 探したい値
// 戻り値: targetが見つかった場合はそのインデックス（0始まり。複数該当する
//         場合はいずれかのインデックス）、見つからなければ-1
int binarySearch(const std::vector<int>& arr, int target);

#endif // BINARY_SEARCH_H
