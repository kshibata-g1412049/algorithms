#ifndef INSERTION_SORT_H
#define INSERTION_SORT_H

#include <vector>

// 挿入ソート（Insertion Sort）
//
// 配列の先頭からソート済み区間を少しずつ拡大していくアルゴリズム。
// 未確定区間の先頭要素を取り出し、ソート済み区間の中で正しい位置まで
// 後方の要素をずらしながら挿入していく。トランプの手札を並べる動作に近い。
//
// 時間複雑度: 最良 O(n)（既にソート済みの場合）、平均/最悪 O(n^2)
// 空間複雑度: O(1)（インプレースソート）
//
// arr: ソート対象の配列（破壊的に変更される）
void insertionSort(std::vector<int>& arr);

#endif // INSERTION_SORT_H
