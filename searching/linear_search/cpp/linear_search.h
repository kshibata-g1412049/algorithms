#ifndef LINEAR_SEARCH_H
#define LINEAR_SEARCH_H

#include <vector>

// 線形探索（Linear Search）
//
// 配列の先頭から末尾まで要素を1つずつ順番に比較していき、
// targetと一致する要素が見つかればそのインデックスを返す。
// 配列がソートされている必要はなく、任意の配列に適用できる。
//
// 時間複雑度: 最良 O(1)（先頭要素が一致する場合）
//             平均/最悪 O(n)
// 空間複雑度: O(1)
//
// arr: 探索対象の配列（ソート不要）
// target: 探したい値
// 戻り値: targetが見つかった場合はそのインデックス（0始まり。複数該当する
//         場合はいずれかのインデックス）、見つからなければ-1
int linearSearch(const std::vector<int>& arr, int target);

#endif // LINEAR_SEARCH_H
