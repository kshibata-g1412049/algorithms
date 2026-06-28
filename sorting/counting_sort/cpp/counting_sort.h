#ifndef COUNTING_SORT_H
#define COUNTING_SORT_H

#include <vector>

// 計数ソート（Counting Sort）
//
// 比較によらないソートアルゴリズム。各値が何回出現するかを数え上げる
// カウント配列を作り、その累積和から各値を配置すべき位置を求めて
// 出力配列を構築する。値の範囲kが小さいほど高速になる。
//
// 制約: 本実装は非負整数（0以上の整数）のみを対象とする。
//
// 時間複雑度: O(n + k)（nは要素数、kは配列中の最大値）
// 空間複雑度: O(n + k)（カウント配列と出力用の配列を必要とする）
//
// arr: ソート対象の配列（非負整数のみを想定。破壊的に変更される）
void countingSort(std::vector<int>& arr);

#endif // COUNTING_SORT_H
