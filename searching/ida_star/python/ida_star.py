"""IDA*探索（Iterative Deepening A*）の実装。

A*の最適性を保ちつつ、open/closedリストのような大きな補助データ構造を
使わずに反復深化（iterative deepening）の考え方で経路を探索するアルゴ
リズム。各反復では「コスト上限値(bound)」を1つ決め、f(n) = g(n) + h(n)
がその上限を超えるノードを枝刈りしながら深さ優先探索(DFS)を行う。
その反復でゴールに到達できなければ、枝刈りされた中で最小のf値を次の
上限値として再度DFSをやり直す。h(n)が許容的（admissible）であれば、
ゴールに到達した時点でのコストは常に最適解と一致する。

closedリストを持たないため、現在のDFSパス上に既にあるノードへの
再訪問のみを禁止する（パス自体を訪問済み集合として使う）。これにより
同一パス内での巡回は防げるが、closedリストを使うA*と異なり、異なる
パスから同じノードへ重複して到達する探索は防げない。そのためIDA*は
最悪ケースでは指数時間となるが、メモリ使用量はO(経路長)と非常に小さい。

本実装ではヒューリスティックとして Octile distance を用いる。
    dx = |r1 - r2|, dy = |c1 - c2|
    h = max(dx, dy) + (sqrt(2) - 1) * min(dx, dy)
これは8方向移動（直進コスト1、斜め移動コストsqrt(2)）に対して
許容的なヒューリスティックである。

移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
その両側の直交マスのいずれかが障害物の場合は斜め移動できない。

時間複雑度: 最悪ケースで指数時間（closedリストを持たないため重複探索
            が発生し得る）。反復ごとにDFSをやり直すコストもあるが、
            深さが浅い反復のノード数は深い反復に比べて少ないため、
            総コストは最終反復のノード数と同程度のオーダーになる。
空間複雑度: O(d)（d=現在のDFSパスの深さ。A*のようなopen/closedリストを
            保持しないため、A*やJump Point Searchより大幅に少ない）
"""
import math
from typing import List, Optional, Set, Tuple

_DIRECTIONS = [(-1, 0), (1, 0), (0, -1), (0, 1), (-1, -1), (-1, 1), (1, -1), (1, 1)]

_SQRT2 = math.sqrt(2)
_SQRT2_MINUS_1 = _SQRT2 - 1.0

_FOUND = -1.0  # ゴールに到達したことを示すセンチネル値


def _octile_distance(a: Tuple[int, int], b: Tuple[int, int]) -> float:
    dx = abs(a[0] - b[0])
    dy = abs(a[1] - b[1])
    return max(dx, dy) + _SQRT2_MINUS_1 * min(dx, dy)


def _in_bounds(grid: List[List[int]], row: int, col: int) -> bool:
    return 0 <= row < len(grid) and 0 <= col < len(grid[0])


def _is_walkable(grid: List[List[int]], row: int, col: int) -> bool:
    return grid[row][col] == 0


def _can_move(grid: List[List[int]], row: int, col: int, dr: int, dc: int) -> bool:
    """(row,col)から方向(dr,dc)へ移動できるかを判定する（コーナーカット禁止含む）。"""
    new_row, new_col = row + dr, col + dc
    if not _in_bounds(grid, new_row, new_col) or not _is_walkable(grid, new_row, new_col):
        return False

    if dr != 0 and dc != 0:
        if not _in_bounds(grid, row + dr, col) or not _is_walkable(grid, row + dr, col):
            return False
        if not _in_bounds(grid, row, col + dc) or not _is_walkable(grid, row, col + dc):
            return False

    return True


def _step_cost(dr: int, dc: int) -> float:
    return _SQRT2 if dr != 0 and dc != 0 else 1.0


def _search(
    grid: List[List[int]],
    path: List[Tuple[int, int]],
    on_path: Set[Tuple[int, int]],
    g: float,
    bound: float,
    goal: Tuple[int, int],
) -> float:
    """現在のDFSパスpathの末尾ノードから探索を1段進める。

    戻り値:
        _FOUND      ゴールに到達した（pathはゴールまでの経路を保持したまま）。
        _FOUND以外  このブランチで枝刈りされた場合に観測された最小のf値
                    （次の反復のboundの候補）。何も枝刈りされなければ
                    +infinityを返す。
    """
    node = path[-1]
    f = g + _octile_distance(node, goal)
    if f > bound:
        return f  # この先は探索しても無駄: 上限を超えた候補値として返す

    if node == goal:
        return _FOUND

    min_exceeded = math.inf

    for dr, dc in _DIRECTIONS:
        if not _can_move(grid, node[0], node[1], dr, dc):
            continue

        neighbor = (node[0] + dr, node[1] + dc)
        if neighbor in on_path:
            continue  # 現在のDFSパス上に既にあるノードは再訪問しない（巡回防止）

        path.append(neighbor)
        on_path.add(neighbor)

        result = _search(grid, path, on_path, g + _step_cost(dr, dc), bound, goal)
        if result == _FOUND:
            return _FOUND  # 見つかった経路はpathに残したまま即座に伝播する

        path.pop()
        on_path.discard(neighbor)
        min_exceeded = min(min_exceeded, result)

    return min_exceeded


def ida_star(
    grid: List[List[int]], start: Tuple[int, int], goal: Tuple[int, int]
) -> List[Tuple[int, int]]:
    """startからgoalまでの最短経路をIDA*で求める。

    Args:
        grid: 2次元配列。0=通行可能, 1=障害物。
        start: 探索開始地点 (row, col)。
        goal: 目的地 (row, col)。

    Returns:
        startからgoalまでの最短経路（両端を含む）を(row,col)のリストで返す。
        到達不可能な場合は空リストを返す。

    事前条件: start/goalはグリッド範囲内かつ通行可能マスであること
              （呼び出し側の責務。本関数内では検証しない）。
    """
    if not grid or not grid[0]:
        return []

    if start == goal:
        return [start]

    bound = _octile_distance(start, goal)

    path: List[Tuple[int, int]] = [start]
    on_path: Set[Tuple[int, int]] = {start}

    while True:
        result = _search(grid, path, on_path, 0.0, bound, goal)
        if result == _FOUND:
            return list(path)
        if not math.isfinite(result):
            return []  # どの分岐も上限を超えなかった = ゴールは到達不可能
        bound = result
