"""挿入ソート（Insertion Sort）の実装。

配列の先頭からソート済み区間を少しずつ拡大していくアルゴリズム。
未確定区間の先頭要素を取り出し、ソート済み区間の中で正しい位置まで
後方の要素をずらしながら挿入していく。トランプの手札を並べる動作に近い。

時間複雑度: 最良 O(n)（既にソート済みの場合）、平均/最悪 O(n^2)
空間複雑度: O(n)（入力を破壊せず新しいリストを返すため）
"""
from typing import List


def insertion_sort(arr: List[int]) -> List[int]:
    """配列を昇順にソートした新しいリストを返す（入力は変更しない）。"""
    result = list(arr)  # 入力を破壊しないようにコピーを作成

    for i in range(1, len(result)):
        key = result[i]  # 挿入対象の値
        j = i

        # ソート済み区間 [0, i) の中で key より大きい要素を後ろにずらす
        while j > 0 and result[j - 1] > key:
            result[j] = result[j - 1]
            j -= 1

        result[j] = key  # 正しい位置にkeyを挿入

    return result
