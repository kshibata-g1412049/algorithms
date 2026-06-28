"""クイックソート（Quick Sort）の実装。

配列から基準値（ピボット）を選び、ピボットより小さい要素を左側、大きい要素を
右側に振り分ける「分割（partition）」を行い、左右の部分配列をそれぞれ再帰的に
ソートする分割統治法に基づくアルゴリズム。本実装ではLomutoのパーティション方式を
用い、区間の末尾要素をピボットとする。

時間複雑度: 最良/平均 O(n log n)、最悪 O(n^2)（ピボットの選び方が常に偏る場合）
空間複雑度: O(log n)（再帰呼び出しのスタック分）
"""
from typing import List


def quick_sort(arr: List[int]) -> List[int]:
    """配列を昇順にソートした新しいリストを返す（入力は変更しない）。"""
    result = list(arr)  # 入力を破壊しないようにコピーを作成
    _quick_sort_range(result, 0, len(result) - 1)
    return result


def _quick_sort_range(arr: List[int], low: int, high: int) -> None:
    """区間 [low, high] を再帰的にクイックソートする（arrを直接変更する）。"""
    if low >= high:
        return  # 要素数が0または1なら既にソート済み

    pivot_index = _partition(arr, low, high)
    _quick_sort_range(arr, low, pivot_index - 1)
    _quick_sort_range(arr, pivot_index + 1, high)


def _partition(arr: List[int], low: int, high: int) -> int:
    """Lomutoパーティション方式: 区間 [low, high] の末尾要素をピボットとし、
    ピボットより小さい要素を左側に集める。最終的なピボットの位置を返す。
    """
    pivot = arr[high]
    i = low - 1  # ピボットより小さい要素の末尾位置

    for j in range(low, high):
        if arr[j] < pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]

    arr[i + 1], arr[high] = arr[high], arr[i + 1]  # ピボットを正しい位置に移動
    return i + 1
