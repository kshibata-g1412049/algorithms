"""指数探索（Exponential Search）の実装。

探索範囲を1, 2, 4, 8, ... と倍々に広げていき、target以上の値を持つ要素が
現れた区間（あるいは配列の終端）を特定する。その後、特定した区間
[prev, curr] に対して二分探索を行う。範囲が分かっている配列やtargetが
先頭に近い場合に有効で、特に上限が不明な（無限に続く）配列に対する探索にも
応用できる。

前提条件: 入力arrは昇順にソート済みであること（事前条件の検証は行わない）。

時間複雑度: O(log n)
空間複雑度: O(1)
"""
from typing import List


def _binary_search_range(arr: List[int], low: int, high: int, target: int) -> int:
    """[low, high] の範囲に対する標準的な二分探索（内部使用）"""
    while low <= high:
        mid = (low + high) // 2
        if arr[mid] == target:
            return mid
        if arr[mid] < target:
            low = mid + 1
        else:
            high = mid - 1
    return -1


def exponential_search(arr: List[int], target: int) -> int:
    """targetが見つかった場合はそのインデックス（0始まり。複数該当する場合は
    いずれかのインデックス）、見つからなければ-1を返す。"""
    n = len(arr)
    if n == 0:
        return -1

    if arr[0] == target:
        return 0

    # 探索範囲を1, 2, 4, 8, ... と倍々に広げ、target以上の値を持つ要素が
    # 現れる区間（または配列の終端）を特定する
    bound = 1
    while bound < n and arr[bound] < target:
        bound *= 2

    # 特定した区間 [bound//2, min(bound, n-1)] に対して二分探索を行う
    low = bound // 2
    high = min(bound, n - 1)
    return _binary_search_range(arr, low, high, target)
