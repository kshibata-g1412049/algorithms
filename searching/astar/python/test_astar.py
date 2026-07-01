"""astar関数のテスト（pytest）。

このファイルは同じディレクトリ内のastar.pyのみに依存しているため、
python/フォルダを単独で取り出してもこのテストは実行できる。
"""
import math

from astar import astar

_SQRT2 = math.sqrt(2)
_EPS = 1e-6


def _path_cost(path):
    """経路の総コスト（直進1・斜めsqrt(2)の合計）を計算する。"""
    cost = 0.0
    for i in range(1, len(path)):
        dr = abs(path[i][0] - path[i - 1][0])
        dc = abs(path[i][1] - path[i - 1][1])
        cost += _SQRT2 if dr == 1 and dc == 1 else 1.0
    return cost


def _is_valid_path(grid, path, start, goal):
    """経路が制約（先頭/末尾、隣接性、障害物回避、コーナーカット禁止）を満たすか検証する。"""
    if not path:
        return False
    if path[0] != start or path[-1] != goal:
        return False

    rows, cols = len(grid), len(grid[0])
    for r, c in path:
        if not (0 <= r < rows and 0 <= c < cols):
            return False
        if grid[r][c] != 0:
            return False

    for i in range(1, len(path)):
        dr = path[i][0] - path[i - 1][0]
        dc = path[i][1] - path[i - 1][1]
        if abs(dr) > 1 or abs(dc) > 1 or (dr == 0 and dc == 0):
            return False
        if dr != 0 and dc != 0:
            r0, c0 = path[i - 1]
            if grid[r0 + dr][c0] != 0 or grid[r0][c0 + dc] != 0:
                return False

    return True


def test_no_obstacles_diagonal_path():
    """障害物なしの直線（斜め）経路は4*sqrt(2)になる"""
    grid = [[0] * 5 for _ in range(5)]
    start, goal = (0, 0), (4, 4)
    path = astar(grid, start, goal)
    assert _is_valid_path(grid, path, start, goal)
    assert abs(_path_cost(path) - 4 * _SQRT2) < _EPS


def test_wall_with_gap_detour():
    """壁に迂回が必要なケース。最短コストは8.0"""
    grid = [
        [0, 0, 0, 0, 0],
        [0, 1, 1, 1, 0],
        [0, 0, 0, 0, 0],
        [0, 1, 1, 1, 0],
        [0, 0, 0, 0, 0],
    ]
    start, goal = (0, 0), (4, 4)
    path = astar(grid, start, goal)
    assert _is_valid_path(grid, path, start, goal)
    assert abs(_path_cost(path) - 8.0) < _EPS


def test_start_equals_goal():
    """startとgoalが同一の場合は[start]のみを返す"""
    grid = [[0] * 3 for _ in range(3)]
    path = astar(grid, (1, 1), (1, 1))
    assert path == [(1, 1)]


def test_unreachable_goal():
    """到達不可能な場合は空リストを返す"""
    grid = [
        [0, 0, 0, 0, 0],
        [1, 1, 1, 1, 1],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0],
    ]
    assert astar(grid, (0, 0), (4, 4)) == []


def test_corner_cut_blocked():
    """斜め移動のコーナーカットが禁止されているため到達不可能になるケース"""
    grid = [
        [0, 1, 0],
        [1, 0, 1],
        [0, 1, 0],
    ]
    assert astar(grid, (0, 0), (2, 2)) == []
