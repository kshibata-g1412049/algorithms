#ifndef QUICK_SORT_H
#define QUICK_SORT_H

#include <vector>

// クイックソート（Quick Sort）
//
// 配列から基準値（ピボット）を選び、ピボットより小さい要素を左側、
// 大きい要素を右側に振り分ける「分割（partition）」を行い、
// 左右の部分配列をそれぞれ再帰的にソートする分割統治法に基づくアルゴリズム。
// 本実装ではLomutoのパーティション方式を用い、区間の末尾要素をピボットとする。
//
// 時間複雑度: 最良/平均 O(n log n)、最悪 O(n^2)（ピボットの選び方が常に偏る場合）
// 空間複雑度: O(log n)（再帰呼び出しのスタック分。データ自体はインプレースソート）
//
// arr: ソート対象の配列（破壊的に変更される）
void quickSort(std::vector<int>& arr);

#endif // QUICK_SORT_H
