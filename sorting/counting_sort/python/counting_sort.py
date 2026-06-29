"""計数ソート（Counting Sort）の実装。

比較によらないソートアルゴリズム。各値が何回出現するかを数え上げる
カウントリストを作り、その累積和から各値を配置すべき位置を求めて
出力リストを構築する。値の範囲kが小さいほど高速になる。

制約: 本実装は非負整数（0以上の整数）のみを対象とする。

時間複雑度: O(n + k)（nは要素数、kは配列中の最大値）
空間複雑度: O(n + k)（カウント配列と出力用の配列を必要とする）
"""
from typing import List


def counting_sort(arr: List[int]) -> List[int]:
    """配列（非負整数のみ）を昇順にソートした新しいリストを返す（入力は変更しない）。"""
    if not arr:
        return []

    # 配列中の最大値を求め、カウントリストのサイズを決める
    max_value = max(arr)
    count = [0] * (max_value + 1)

    # 各値の出現回数を数え上げる
    for value in arr:
        count[value] += 1

    # 累積和を取ることで、各値が出力リストの何番目までの位置に収まるかを求める
    for i in range(1, len(count)):
        count[i] += count[i - 1]

    # 累積和を使って出力リストを構築する（後ろから走査することで安定性を保つ）
    output = [0] * len(arr)
    for value in reversed(arr):
        count[value] -= 1
        output[count[value]] = value

    return output
