"""重み付きA*探索（Weighted A* search）の実装。

通常のA*は f(n) = g(n) + h(n) でノードを評価するが、重み付きA*では
f(n) = g(n) + weight * h(n) としてヒューリスティックの影響を強める。
weight > 1.0 にするとヒューリスティックがより強く効くため、ゴール方向への
探索が加速し展開ノード数が減って高速になるが、ヒューリスティックが
許容的でなくなる（実コストを過大評価しうる）ため最適性は保証されない。
weight = 1.0 のときは通常のA*と等価になる。

本実装ではヒューリスティックとして Octile distance を用いる。
    dx = |r1 - r2|, dy = |c1 - c2|
    h = max(dx, dy) + (sqrt(2) - 1) * min(dx, dy)

移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
その両側の直交マスのいずれかが障害物の場合は斜め移動できない。

時間複雑度: O(V log V + E)（V=セル数、E=辺数。実際にはweightが大きいほど
            展開ノード数が減るため、通常のA*より高速になる傾向がある）
空間複雑度: O(V)
"""
import heapq
import math
from typing import Dict, List, Tuple

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


def weighted_astar(
    grid: List[List[int]], start: Tuple[int, int], goal: Tuple[int, int], weight: float
) -> List[Tuple[int, int]]:
    """startからgoalまでの経路を重み付きA*で求める。

    Args:
        grid: 2次元配列。0=通行可能, 1=障害物。
        start: 探索開始地点 (row, col)。
        goal: 目的地 (row, col)。
        weight: ヒューリスティックに掛ける重み（>1.0で高速化、最適性は保証されない）。

    Returns:
        startからgoalまでの経路（両端を含む）を(row,col)のリストで返す。
        到達不可能な場合は空リストを返す。経路の総コストはweight倍した
        理論上限（weight * 最適コスト）以下になる。

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

    open_heap = [(weight * _octile_distance(start, goal), start)]

    while open_heap:
        _, current = heapq.heappop(open_heap)

        if closed.get(current, False):
            continue
        closed[current] = True

        if current == goal:
            break

        row, col = current
        current_g = g_score[current]

        for dr, dc in _DIRECTIONS:
            if not _can_move(grid, row, col, dr, dc):
                continue

            neighbor = (row + dr, col + dc)
            if closed.get(neighbor, False):
                continue

            tentative_g = current_g + _step_cost(dr, dc)
            if neighbor not in g_score or tentative_g < g_score[neighbor]:
                g_score[neighbor] = tentative_g
                parent[neighbor] = current
                # f(n) = g(n) + weight * h(n)
                f = tentative_g + weight * _octile_distance(neighbor, goal)
                heapq.heappush(open_heap, (f, neighbor))

    if goal not in g_score or not closed.get(goal, False):
        return []

    path = [goal]
    current = goal
    while current != start:
        current = parent[current]
        path.append(current)
    path.reverse()
    return path
