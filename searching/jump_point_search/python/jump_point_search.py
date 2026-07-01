"""Jump Point Search (JPS) の実装。

グリッド上のA*探索を高速化したアルゴリズム。8方向の均一コストグリッド
という前提（直進コスト1、斜めコストsqrt(2)）を利用し、対称性を持つ
経路を1セルずつ展開するのではなく、「強制近傍(forced neighbor)」が
現れるセルまで一気に飛ばして（ジャンプして）探索する。これにより
探索するノード数がA*より大幅に減ることが多い。

近傍の刈り込み(pruning): 親ノードからの進入方向(dr,dc)に応じて、まず
自然近傍（直進方向、斜め進入ならその水平・垂直成分）を候補に入れる。
加えて「強制近傍」、すなわち1手前のセルからは到達できなかった方向が
現在のセルからは到達できるようになる方向（Harabor & Grastien, 2011の
定義をコーナーカット禁止のcan_moveにそのまま適用したもの）を候補に
加える。これにより最短経路の最適性を保ったまま探索すべき方向を
大幅に絞り込む。コーナーカット許容前提でハードコードされた教科書通りの
障害物判定（側方セルが障害物なら強制近傍、等）はコーナーカット禁止下では
判定条件と提案される斜め移動が両立しない（判定に使う障害物セル自体が
can_moveのコーナーカット判定対象セルと一致してしまう）ことがあり、
必要な近傍を取り逃すため採用していない。

ジャンプ(jump)関数: ある方向に直進し続け、(a)障害物または範囲外に
達した場合は失敗、(b)ゴールに達した場合は成功、(c)強制近傍を持つ
セルに達した場合は成功、を判定する。斜め方向のジャンプでは、各ステップ
で水平・垂直方向のサブジャンプも試行し、いずれかが成功すれば現在地点
もジャンプポイントとなる。

移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
その両側の直交マスのいずれかが障害物の場合は斜め移動できない。

ヒューリスティック: Octile distance（A*系列の他アルゴリズムと共通）。

時間複雑度: 最悪ケースはA*と同じO(V log V + E)だが、実用上は
            ジャンプによって展開ノード数が大幅に減るため高速。
空間複雑度: O(V)
"""
import heapq
import math
from typing import Dict, List, Optional, Tuple

_DIRECTIONS = [(-1, 0), (1, 0), (0, -1), (0, 1), (-1, -1), (-1, 1), (1, -1), (1, 1)]

_SQRT2 = math.sqrt(2)
_SQRT2_MINUS_1 = _SQRT2 - 1.0


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


def _is_natural_direction(dr: int, dc: int, ddr: int, ddc: int) -> bool:
    """entry方向(dr,dc)に対する自然近傍（直進・斜め進入の主成分）かどうか。"""
    if dr != 0 and dc != 0:
        return (ddr == dr and ddc == 0) or (ddr == 0 and ddc == dc) or (ddr == dr and ddc == dc)
    return ddr == dr and ddc == dc


def _forced_neighbor_directions(
    grid: List[List[int]], row: int, col: int, dr: int, dc: int
) -> List[Tuple[int, int]]:
    """(row,col)に方向(dr,dc)で進入した際の強制近傍方向の一覧を返す。

    Harabor & Grastienによる強制近傍の定義「1手前のセルからは到達できな
    かった近傍が、現在のセルからは到達できる」を、コーナーカット禁止を
    含む_can_moveに対してそのまま適用する。後退方向（来た方向の正反対）
    のみを除外し、その他の7方向は新規性のみで判定する。
    """
    prev_row, prev_col = row - dr, col - dc
    prev_in_bounds = _in_bounds(grid, prev_row, prev_col)

    result = []
    for ddr, ddc in _DIRECTIONS:
        if _is_natural_direction(dr, dc, ddr, ddc):
            continue
        if ddr == -dr and ddc == -dc:
            continue  # 正反対の後退方向は最短経路に寄与しないため除外
        if not _can_move(grid, row, col, ddr, ddc):
            continue
        prev_could_move = prev_in_bounds and _can_move(grid, prev_row, prev_col, ddr, ddc)
        if not prev_could_move:
            result.append((ddr, ddc))
    return result


def _has_forced_neighbor(grid: List[List[int]], row: int, col: int, dr: int, dc: int) -> bool:
    return len(_forced_neighbor_directions(grid, row, col, dr, dc)) > 0


def _jump(
    grid: List[List[int]],
    row: int,
    col: int,
    dr: int,
    dc: int,
    goal: Tuple[int, int],
) -> Optional[Tuple[int, int]]:
    """方向(dr,dc)へ直進し続け、ジャンプポイント（強制近傍を持つセル・ゴール・
    それ以上進めない行き止まりのセル）を探す。最初の一歩目で進めない場合
    （すぐに障害物・範囲外に当たった場合）のみNoneを返す。1歩以上進めた後に
    行き止まりになった場合は、その地点を曲がるためのジャンプポイントとして
    返す（グリッド境界で直角に曲がる必要がある経路を取り逃さないため）。
    """
    if not _can_move(grid, row, col, dr, dc):
        return None

    cur_row, cur_col = row + dr, col + dc

    while True:
        if (cur_row, cur_col) == goal:
            return (cur_row, cur_col)

        if _has_forced_neighbor(grid, cur_row, cur_col, dr, dc):
            return (cur_row, cur_col)

        if dr != 0 and dc != 0:
            # 斜め移動の場合、各ステップで水平・垂直方向のサブジャンプも試す。
            if _jump(grid, cur_row, cur_col, dr, 0, goal) is not None:
                return (cur_row, cur_col)
            if _jump(grid, cur_row, cur_col, 0, dc, goal) is not None:
                return (cur_row, cur_col)

        if not _can_move(grid, cur_row, cur_col, dr, dc):
            return (cur_row, cur_col)
        cur_row += dr
        cur_col += dc


def _pruned_directions(
    grid: List[List[int]],
    row: int,
    col: int,
    parent: Optional[Tuple[int, int]],
) -> List[Tuple[int, int]]:
    """現在ノードについて、探索すべき方向(dr,dc)の集合を刈り込んで返す。
    parentがNoneの場合（start地点）は8方向すべてを候補とする。
    """
    if parent is None:
        return [d for d in _DIRECTIONS if _can_move(grid, row, col, d[0], d[1])]

    dr = (row > parent[0]) - (row < parent[0])
    dc = (col > parent[1]) - (col < parent[1])

    dirs: List[Tuple[int, int]] = []

    # 自然近傍（直進方向、斜め進入ならその水平・垂直成分も含む）。
    if dr != 0 and dc != 0:
        if _can_move(grid, row, col, dr, 0):
            dirs.append((dr, 0))
        if _can_move(grid, row, col, 0, dc):
            dirs.append((0, dc))
    if _can_move(grid, row, col, dr, dc):
        dirs.append((dr, dc))

    # 強制近傍（_has_forced_neighborと同じ「1手前では不可、現在は可能」の定義）。
    dirs.extend(_forced_neighbor_directions(grid, row, col, dr, dc))

    return dirs


def jump_point_search(
    grid: List[List[int]], start: Tuple[int, int], goal: Tuple[int, int]
) -> List[Tuple[int, int]]:
    """startからgoalまでの最短経路をJump Point Searchで求める。

    Args:
        grid: 2次元配列。0=通行可能, 1=障害物。
        start: 探索開始地点 (row, col)。
        goal: 目的地 (row, col)。

    Returns:
        startからgoalまでの最短経路を(row,col)のリストで返す
        （start, goal両端を含み、ジャンプポイント間も1セルずつ展開した
        連続経路として返す）。到達不可能な場合は空リストを返す。

    事前条件: start/goalはグリッド範囲内かつ通行可能マスであること
              （呼び出し側の責務。本関数内では検証しない）。
    """
    if not grid or not grid[0]:
        return []

    if start == goal:
        return [start]

    g_score: Dict[Tuple[int, int], float] = {start: 0.0}
    parent: Dict[Tuple[int, int], Tuple[int, int]] = {}
    closed: Dict[Tuple[int, int], bool] = {}

    open_heap = [(_octile_distance(start, goal), start)]

    found = False

    while open_heap:
        _, current = heapq.heappop(open_heap)

        if closed.get(current, False):
            continue
        closed[current] = True

        if current == goal:
            found = True
            break

        row, col = current
        current_g = g_score[current]
        parent_pos = parent.get(current)

        for dr, dc in _pruned_directions(grid, row, col, parent_pos):
            jp = _jump(grid, row, col, dr, dc, goal)
            if jp is None:
                continue
            if closed.get(jp, False):
                continue

            steps = max(abs(jp[0] - row), abs(jp[1] - col))
            tentative_g = current_g + steps * _step_cost(dr, dc)

            if jp not in g_score or tentative_g < g_score[jp]:
                g_score[jp] = tentative_g
                parent[jp] = current
                f = tentative_g + _octile_distance(jp, goal)
                heapq.heappush(open_heap, (f, jp))

    if not found:
        return []

    # ジャンプポイントの列をたどって経路を復元する。
    jump_points = [goal]
    current = goal
    while current != start:
        current = parent[current]
        jump_points.append(current)
    jump_points.reverse()

    # ジャンプポイント間は直線（直進または斜め45度）でつながっているため、
    # 1セルずつ展開して連続経路を構築する。
    path = [jump_points[0]]
    for i in range(1, len(jump_points)):
        from_row, from_col = jump_points[i - 1]
        to_row, to_col = jump_points[i]
        dr = (to_row > from_row) - (to_row < from_row)
        dc = (to_col > from_col) - (to_col < from_col)
        cur_row, cur_col = from_row, from_col
        while (cur_row, cur_col) != (to_row, to_col):
            cur_row += dr
            cur_col += dc
            path.append((cur_row, cur_col))

    return path
