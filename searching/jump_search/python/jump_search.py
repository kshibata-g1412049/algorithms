"""ジャンプ探索（Jump Search）の実装。

ソート済み配列を一定のブロックサイズ（一般的には√n）でジャンプしながら
走査し、targetが含まれている可能性のあるブロックを特定する。ブロックの
末尾要素がtarget以上になった時点でジャンプを止め、そのブロックの内部を
先頭から線形探索する。二分探索よりブロックを「戻る」コストが低く、
線形探索よりジャンプによって走査範囲を絞れる点が特徴。

前提条件: 入力arrは昇順にソート済みであること（事前条件の検証は行わない）。

時間複雑度: O(√n)
空間複雑度: O(1)
"""
import math
from typing import List


def jump_search(arr: List[int], target: int) -> int:
    """targetが見つかった場合はそのインデックス（0始まり。複数該当する場合は
    いずれかのインデックス）、見つからなければ-1を返す。"""
    n = len(arr)
    if n == 0:
        return -1

    # ブロックサイズは√nが理論上最適（ジャンプ回数と線形探索回数のバランスが取れる）
    step = int(math.sqrt(n))

    prev = 0
    curr = step

    # target以上の値を持つブロックが見つかるまでジャンプを続ける
    while curr < n and arr[curr - 1] < target:
        prev = curr
        curr += step

    # 特定したブロック [prev, min(curr, n)) を先頭から線形探索する
    end = min(curr, n)
    for i in range(prev, end):
        if arr[i] == target:
            return i

    return -1
