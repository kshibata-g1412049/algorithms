#ifndef SHELL_SORT_H
#define SHELL_SORT_H

#include <vector>

// シェルソート（Shell Sort）
//
// 挿入ソートを拡張したアルゴリズム。最初は大きく離れた要素同士
// （gap=n/2など）を比較・交換して大まかに整列させ、gapを徐々に小さくして
// 最終的にgap=1（通常の挿入ソート相当）まで繰り返すことで、要素の
// 移動距離を減らし挿入ソートより高速化を図る。本実装ではgapをn/2から
// 開始し毎回半分にする方式（Shellの原論文のgap列）を用いる。
//
// 時間複雑度: gap列に依存。本実装（gap=n/2,n/4,...,1）では平均/最悪 O(n^2)、
//             より工夫したgap列を用いるとO(n log^2 n)程度まで改善できる
// 空間複雑度: O(1)（インプレースソート）
//
// arr: ソート対象の配列（破壊的に変更される）
void shellSort(std::vector<int>& arr);

#endif // SHELL_SORT_H
