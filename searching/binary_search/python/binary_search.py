"""二分探索（Binary Search）の実装。

探索範囲の中央要素とtargetを比較し、targetが中央要素より小さければ左半分、
大きければ右半分に絞り込むことを繰り返す。比較を1回行うたびに探索範囲が
半分になるため、非常に高速に探索できる。

前提条件: 入力arrは昇順にソート済みであること（事前条件の検証は行わない）。

時間複雑度: O(log n)
空間複雑度: O(1)（反復実装のため）
"""
from typing import List


def binary_search(arr: List[int], target: int) -> int:
    """targetが見つかった場合はそのインデックス（0始まり。複数該当する場合は
    いずれかのインデックス）、見つからなければ-1を返す。"""
    low, high = 0, len(arr) - 1

    while low <= high:
        mid = (low + high) // 2

        if arr[mid] == target:
            return mid

        if arr[mid] < target:
            low = mid + 1  # 右半分に絞り込む
        else:
            high = mid - 1  # 左半分に絞り込む

    return -1
