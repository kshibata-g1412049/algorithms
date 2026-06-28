#ifndef SELECTION_SORT_H
#define SELECTION_SORT_H

#include <vector>

// 選択ソート（Selection Sort）
//
// 未確定区間の中から最小値を探し、それを未確定区間の先頭要素と交換する、
// という操作を繰り返すことで配列の先頭から順にソート済み区間を
// 拡大していくアルゴリズム。交換回数が最大でも n-1 回に抑えられる点が特徴。
//
// 時間複雑度: 最良/平均/最悪 すべて O(n^2)（最小値探索は常に全区間を見るため）
// 空間複雑度: O(1)（インプレースソート）
//
// arr: ソート対象の配列（破壊的に変更される）
void selectionSort(std::vector<int>& arr);

#endif // SELECTION_SORT_H
