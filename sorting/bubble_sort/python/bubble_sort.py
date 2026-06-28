"""バブルソート（Bubble Sort）の実装。

隣り合う要素を比較し、順序が逆であれば交換するという操作を配列の先頭から
末尾まで繰り返すことで、1回のパスごとに未確定の最大値を末尾側に
「泡（バブル）」のように浮かび上がらせていくアルゴリズム。

時間複雑度: 最良 O(n)（既にソート済みの場合は早期終了する）、平均/最悪 O(n^2)
空間複雑度: O(n)（入力を破壊せず新しいリストを返すため）
"""
from typing import List


def bubble_sort(arr: List[int]) -> List[int]:
    """配列を昇順にソートした新しいリストを返す（入力は変更しない）。"""
    result = list(arr)  # 入力を破壊しないようにコピーを作成
    n = len(result)

    for i in range(n - 1):
        swapped = False  # このパスで交換が発生したかどうか

        # 未確定区間 [0, n-1-i) を1回スキャンし、隣接要素を比較・交換する
        for j in range(n - 1 - i):
            if result[j] > result[j + 1]:
                result[j], result[j + 1] = result[j + 1], result[j]
                swapped = True

        # 交換が一度も起きなかった = 既にソート済みなので早期終了
        if not swapped:
            break

    return result
