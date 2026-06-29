"""ヒープソート（Heap Sort）の実装。

配列全体を最大ヒープ（親が子より必ず大きい二分木）として構築したうえで、
ヒープの根（=配列中の最大値）を配列の末尾と交換し、ヒープの範囲を1つ
縮めて再度ヒープ条件を満たすように調整する、という操作を繰り返す
アルゴリズム。これにより配列の末尾から順に確定値が積み上がっていく。

時間複雑度: 最良/平均/最悪 すべて O(n log n)
空間複雑度: O(n)（入力を破壊せず新しいリストを返すため。リスト自体は二分木として扱う）
"""
from typing import List


def heap_sort(arr: List[int]) -> List[int]:
    """配列を昇順にソートした新しいリストを返す（入力は変更しない）。"""
    result = list(arr)  # 入力を破壊しないようにコピーを作成
    n = len(result)

    if n < 2:
        return result

    # 配列全体を最大ヒープに構築する（葉ではない最後の親から逆順に調整）
    for i in range(n // 2 - 1, -1, -1):
        _sift_down(result, n, i)

    # ヒープの根（最大値）を末尾と交換し、ヒープ範囲を1つ縮めて再調整する
    for end in range(n - 1, 0, -1):
        result[0], result[end] = result[end], result[0]
        _sift_down(result, end, 0)

    return result


def _sift_down(arr: List[int], heap_size: int, i: int) -> None:
    """添字iを根とする部分木がヒープ条件を満たすように、根を下方向に沈めて調整する。
    heap_sizeはヒープとして扱う範囲（[0, heap_size)）を表す。
    """
    largest = i
    left = 2 * i + 1
    right = 2 * i + 2

    if left < heap_size and arr[left] > arr[largest]:
        largest = left
    if right < heap_size and arr[right] > arr[largest]:
        largest = right

    if largest != i:
        arr[i], arr[largest] = arr[largest], arr[i]
        _sift_down(arr, heap_size, largest)  # 交換した先でヒープ条件が崩れていないか再帰的に確認
