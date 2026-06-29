#ifndef INTERPOLATION_SEARCH_H
#define INTERPOLATION_SEARCH_H

#include <vector>

// 補間探索（Interpolation Search）
//
// 二分探索が常に中央要素を見るのに対し、補間探索は探索範囲の両端の値と
// targetの値の関係から「targetがありそうな位置」を線形補間によって推定し、
// その位置から比較を始める。データが一様分布に近いほど効率が良くなる。
//
// 前提条件: 入力配列arrは昇順にソート済みであること（事前条件の検証は行わない）。
//
// 時間複雑度: 平均 O(log log n)（データが一様分布に近い場合）
//             最悪 O(n)（データの分布が偏っている場合）
// 空間複雑度: O(1)
//
// arr: 昇順にソート済みの探索対象の配列
// target: 探したい値
// 戻り値: targetが見つかった場合はそのインデックス（0始まり。複数該当する
//         場合はいずれかのインデックス）、見つからなければ-1
int interpolationSearch(const std::vector<int>& arr, int target);

#endif // INTERPOLATION_SEARCH_H
