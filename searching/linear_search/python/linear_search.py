"""線形探索（Linear Search）の実装。

配列の先頭から末尾まで要素を1つずつ順番に比較していき、targetと一致する
要素が見つかればそのインデックスを返す。配列がソートされている必要はなく、
任意の配列に適用できる。

時間複雑度: 最良 O(1)（先頭要素が一致する場合）、平均/最悪 O(n)
空間複雑度: O(1)
"""
from typing import List


def linear_search(arr: List[int], target: int) -> int:
    """targetが見つかった場合はそのインデックス（0始まり。複数該当する場合は
    いずれかのインデックス）、見つからなければ-1を返す。"""
    for i, value in enumerate(arr):
        if value == target:
            return i
    return -1
