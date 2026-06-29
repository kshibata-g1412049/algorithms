"""補間探索（Interpolation Search）の実装。

二分探索が常に中央要素を見るのに対し、補間探索は探索範囲の両端の値と
targetの値の関係から「targetがありそうな位置」を線形補間によって推定し、
その位置から比較を始める。データが一様分布に近いほど効率が良くなる。

前提条件: 入力arrは昇順にソート済みであること（事前条件の検証は行わない）。

時間複雑度: 平均 O(log log n)（データが一様分布に近い場合）、最悪 O(n)
空間複雑度: O(1)
"""
from typing import List


def interpolation_search(arr: List[int], target: int) -> int:
    """targetが見つかった場合はそのインデックス（0始まり。複数該当する場合は
    いずれかのインデックス）、見つからなければ-1を返す。"""
    low, high = 0, len(arr) - 1

    while low <= high:
        if low == high:
            return low if arr[low] == target else -1

        # 全要素が同じ値の場合、分母（arr[high]-arr[low]）が0になり
        # ゼロ除算が発生するため、その場合は線形探索的にlowを直接調べる
        if arr[high] == arr[low]:
            return low if arr[low] == target else -1

        # 両端の値とtargetの値から、targetがありそうな位置を線形補間で推定する
        pos = low + (high - low) * (target - arr[low]) // (arr[high] - arr[low])

        # 推定位置が範囲外になる場合（targetが範囲外の値の場合）は探索失敗
        if pos < low or pos > high:
            return -1

        if arr[pos] == target:
            return pos

        if arr[pos] < target:
            low = pos + 1  # 推定位置より右側に絞り込む
        else:
            high = pos - 1  # 推定位置より左側に絞り込む

    return -1
