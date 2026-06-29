"""マージソート（Merge Sort）の実装。

配列を再帰的に半分に分割し（分割: Divide）、それぞれを再帰的にソートしたうえで
（統治: Conquer）、2つのソート済み配列を先頭から比較しながら1つにまとめる
（結合: Merge）という分割統治法に基づくアルゴリズム。

時間複雑度: 最良/平均/最悪 すべて O(n log n)
空間複雑度: O(n)（マージ時に作業用リストを必要とする）
"""
from typing import List


def merge_sort(arr: List[int]) -> List[int]:
    """配列を昇順にソートした新しいリストを返す（入力は変更しない）。"""
    if len(arr) <= 1:
        return list(arr)  # 要素数が0または1なら既にソート済み

    # 配列を半分に分割し、それぞれを再帰的にソートする
    mid = len(arr) // 2
    left = merge_sort(arr[:mid])
    right = merge_sort(arr[mid:])

    return _merge(left, right)


def _merge(left: List[int], right: List[int]) -> List[int]:
    """2つのソート済みリストを1つのソート済みリストにまとめる。"""
    merged: List[int] = []
    i = j = 0

    # 両リストの先頭同士を比較し、小さい方を順にmergedへ詰めていく
    while i < len(left) and j < len(right):
        if left[i] <= right[j]:
            merged.append(left[i])
            i += 1
        else:
            merged.append(right[j])
            j += 1

    # 残った要素をそのまま詰める（どちらか一方は既に詰め終わっている）
    merged.extend(left[i:])
    merged.extend(right[j:])

    return merged
