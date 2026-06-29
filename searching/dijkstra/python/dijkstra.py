"""ダイクストラ法（Dijkstra's algorithm）の実装。

グリッド上の単一始点最短経路を求めるアルゴリズム。スタート地点からの
実コスト g(n) が最小のノードから順に確定（=優先度付きキューから取り出して
展開）していくことで、各ノードへの最短コストを単調に確定させていく。
ヒューリスティックを使わずに全方向へ均等にコストを伝播するため、
負の辺重みが無い限り常に最適解（最短経路）を返す。

移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
その両側の直交マスのいずれかが障害物の場合は斜め移動できない。

時間複雑度: O(V log V + E)（V=セル数、E=辺数。二分ヒープによる
            優先度付きキューを使用した場合）
空間複雑度: O(V)（距離・親情報・優先度付きキューの保持に必要）
"""
import heapq
import math
from typing import Dict, List, Tuple

_DIRECTIONS = [(-1, 0), (1, 0), (0, -1), (0, 1), (-1, -1), (-1, 1), (1, -1), (1, 1)]
_SQRT2 = math.sqrt(2)


def _in_bounds(grid: List[List[int]], row: int, col: int) -> bool:
    return 0 <= row < len(grid) and 0 <= col < len(grid[0])


def _is_walkable(grid: List[List[int]], row: int, col: int) -> bool:
    return grid[row][col] == 0


def _can_move(grid: List[List[int]], row: int, col: int, dr: int, dc: int) -> bool:
    """(row,col)から方向(dr,dc)へ移動できるかを判定する。

    斜め移動の場合はコーナーカット禁止のルールを適用する：
    両側の直交マスのいずれかが障害物または範囲外であれば移動できない。
    """
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


def dijkstra(
    grid: List[List[int]], start: Tuple[int, int], goal: Tuple[int, int]
) -> List[Tuple[int, int]]:
    """startからgoalまでの最短経路をダイクストラ法で求める。

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

    dist: Dict[Tuple[int, int], float] = {start: 0.0}
    parent: Dict[Tuple[int, int], Tuple[int, int]] = {}
    closed: Dict[Tuple[int, int], bool] = {}

    open_heap = [(0.0, start)]

    while open_heap:
        current_dist, current = heapq.heappop(open_heap)

        if closed.get(current, False):
            continue
        closed[current] = True

        if current == goal:
            break

        row, col = current

        for dr, dc in _DIRECTIONS:
            if not _can_move(grid, row, col, dr, dc):
                continue

            neighbor = (row + dr, col + dc)
            if closed.get(neighbor, False):
                continue

            tentative_dist = current_dist + _step_cost(dr, dc)
            if neighbor not in dist or tentative_dist < dist[neighbor]:
                dist[neighbor] = tentative_dist
                parent[neighbor] = current
                heapq.heappush(open_heap, (tentative_dist, neighbor))

    if goal not in dist or not closed.get(goal, False):
        return []

    path = [goal]
    current = goal
    while current != start:
        current = parent[current]
        path.append(current)
    path.reverse()
    return path
