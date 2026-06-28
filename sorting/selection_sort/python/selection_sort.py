"""選択ソート（Selection Sort）の実装。

未確定区間の中から最小値を探し、それを未確定区間の先頭要素と交換する、
という操作を繰り返すことで配列の先頭から順にソート済み区間を拡大していく
アルゴリズム。交換回数が最大でも n-1 回に抑えられる点が特徴。

時間複雑度: 最良/平均/最悪 すべて O(n^2)（最小値探索は常に全区間を見るため）
空間複雑度: O(n)（入力を破壊せず新しいリストを返すため）
"""
from typing import List


def selection_sort(arr: List[int]) -> List[int]:
    """配列を昇順にソートした新しいリストを返す（入力は変更しない）。"""
    result = list(arr)  # 入力を破壊しないようにコピーを作成
    n = len(result)

    for i in range(n - 1):
        min_index = i  # 未確定区間 [i, n) の中で最小値を持つ要素の添字

        # 未確定区間全体を走査して最小値の位置を探す
        for j in range(i + 1, n):
            if result[j] < result[min_index]:
                min_index = j

        # 最小値を未確定区間の先頭（i番目）に移動する
        if min_index != i:
            result[i], result[min_index] = result[min_index], result[i]

    return result
