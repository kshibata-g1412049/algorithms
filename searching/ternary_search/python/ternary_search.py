"""三分探索（Ternary Search）の実装。

探索範囲を3等分する2つの点mid1, mid2を取り、targetとの比較によって3つの
部分区間のうち探索を続けるべき1つに絞り込む。二分探索が範囲を1/2に絞るのに
対し、三分探索は1/3に絞り込むが、1回の反復で2回の比較を行うため、比較回数の
観点では二分探索より効率が劣ることが多い（定数倍の違い）。

前提条件: 入力arrは昇順にソート済みであること（事前条件の検証は行わない）。

時間複雑度: O(log3 n)
空間複雑度: O(1)（反復実装のため）
"""
from typing import List


def ternary_search(arr: List[int], target: int) -> int:
    """targetが見つかった場合はそのインデックス（0始まり。複数該当する場合は
    いずれかのインデックス）、見つからなければ-1を返す。"""
    low, high = 0, len(arr) - 1

    while low <= high:
        # 探索範囲を3等分する2つの分割点
        mid1 = low + (high - low) // 3
        mid2 = high - (high - low) // 3

        if arr[mid1] == target:
            return mid1
        if arr[mid2] == target:
            return mid2

        if target < arr[mid1]:
            high = mid1 - 1  # 第1区間に絞り込む
        elif target > arr[mid2]:
            low = mid2 + 1  # 第3区間に絞り込む
        else:
            low = mid1 + 1  # 第2区間（中央）に絞り込む
            high = mid2 - 1

    return -1
