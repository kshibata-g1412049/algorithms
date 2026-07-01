#ifndef FIBONACCI_SEARCH_H
#define FIBONACCI_SEARCH_H

#include <vector>

// フィボナッチ探索（Fibonacci Search）
//
// フィボナッチ数列を使って配列を不均等な比率で分割していく探索法。
// 配列サイズn以上となる最小のフィボナッチ数F(k)を求め、F(k-1)とF(k-2)を
// 使って分割位置を決める。除算を使わずに加減算のみで分割位置を計算できる
// ため、除算コストが高い環境（古いCPUなど）では二分探索よりも有利になる
// 場合がある。
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
int fibonacciSearch(const std::vector<int>& arr, int target);

#endif // FIBONACCI_SEARCH_H
