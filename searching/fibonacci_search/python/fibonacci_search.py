"""フィボナッチ探索（Fibonacci Search）の実装。

フィボナッチ数列を使って配列を不均等な比率で分割していく探索法。配列サイズ
n以上となる最小のフィボナッチ数F(k)を求め、F(k-1)とF(k-2)を使って分割位置を
決める。除算を使わずに加減算のみで分割位置を計算できるため、除算コストが
高い環境（古いCPUなど）では二分探索よりも有利になる場合がある。

前提条件: 入力arrは昇順にソート済みであること（事前条件の検証は行わない）。

時間複雑度: O(log n)
空間複雑度: O(1)
"""
from typing import List


def fibonacci_search(arr: List[int], target: int) -> int:
    """targetが見つかった場合はそのインデックス（0始まり。複数該当する場合は
    いずれかのインデックス）、見つからなければ-1を返す。"""
    n = len(arr)
    if n == 0:
        return -1

    # n以上となる最小のフィボナッチ数列 fib_m = fib_m_m1 + fib_m_m2 を求める
    fib_m_m2 = 0  # F(k-2)
    fib_m_m1 = 1  # F(k-1)
    fib_m = fib_m_m2 + fib_m_m1  # F(k)

    while fib_m < n:
        fib_m_m2 = fib_m_m1
        fib_m_m1 = fib_m
        fib_m = fib_m_m2 + fib_m_m1

    # offsetは「まだ調べていない先頭側の要素数」を表す
    offset = -1

    while fib_m > 1:
        # F(k-2)分だけ進んだ位置を比較対象にする（配列範囲を超えないようclamp）
        i = min(offset + fib_m_m2, n - 1)

        if arr[i] < target:
            # targetはiより右側にあるので、[offset, i]の区間を除外し
            # フィボナッチ数列を1つ分小さくして範囲を狭める
            fib_m = fib_m_m1
            fib_m_m1 = fib_m_m2
            fib_m_m2 = fib_m - fib_m_m1
            offset = i
        elif arr[i] > target:
            # targetはiより左側にあるので、フィボナッチ数列を2つ分小さくする
            fib_m = fib_m_m2
            fib_m_m1 = fib_m_m1 - fib_m_m2
            fib_m_m2 = fib_m - fib_m_m1
        else:
            return i

    # 残り1要素（offset+1の位置）を確認する
    if fib_m_m1 == 1 and offset + 1 < n and arr[offset + 1] == target:
        return offset + 1

    return -1
