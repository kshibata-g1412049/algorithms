#ifndef MERGE_SORT_H
#define MERGE_SORT_H

#include <vector>

// マージソート（Merge Sort）
//
// 配列を再帰的に半分に分割し（分割: Divide）、それぞれを再帰的にソートしたうえで
// （統治: Conquer）、2つのソート済み配列を先頭から比較しながら1つにまとめる
// （結合: Merge）という分割統治法に基づくアルゴリズム。
//
// 時間複雑度: 最良/平均/最悪 すべて O(n log n)
// 空間複雑度: O(n)（マージ時に作業用配列を必要とする）
//
// arr: ソート対象の配列（破壊的に変更される）
void mergeSort(std::vector<int>& arr);

#endif // MERGE_SORT_H
