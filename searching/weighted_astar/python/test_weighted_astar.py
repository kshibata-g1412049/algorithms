"""weighted_astar関数のテスト（pytest）。

このファイルは同じディレクトリ内のweighted_astar.pyのみに依存しているため、
python/フォルダを単独で取り出してもこのテストは実行できる。

weighted_astarは最適性を保証しないため、検証内容は以下に限定する：
  - 経路が見つかること
  - 障害物を通らないこと
  - 先頭がstart、末尾がgoalであること
  - コストが理論上限（weight * 最適コスト）以下であること
"""
import math

from weighted_astar import weighted_astar

_SQRT2 = math.sqrt(2)
_EPS = 1e-6


def _path_cost(path):
    cost = 0.0
    for i in range(1, len(path)):
        dr = abs(path[i][0] - path[i - 1][0])
        dc = abs(path[i][1] - path[i - 1][1])
        cost += _SQRT2 if dr == 1 and dc == 1 else 1.0
    return cost


def _is_valid_path(grid, path, start, goal):
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
    """障害物なしの直線（斜め）経路。最適コストは4*sqrt(2)"""
    grid = [[0] * 5 for _ in range(5)]
    start, goal = (0, 0), (4, 4)
    optimal_cost = 4 * _SQRT2
    path = weighted_astar(grid, start, goal, 1.5)
    assert _is_valid_path(grid, path, start, goal)
    assert _path_cost(path) <= 1.5 * optimal_cost + _EPS


def test_wall_with_gap_detour():
    """壁に迂回が必要なケース。最適コストは8.0"""
    grid = [
        [0, 0, 0, 0, 0],
        [0, 1, 1, 1, 0],
        [0, 0, 0, 0, 0],
        [0, 1, 1, 1, 0],
        [0, 0, 0, 0, 0],
    ]
    start, goal = (0, 0), (4, 4)
    optimal_cost = 8.0
    path = weighted_astar(grid, start, goal, 2.0)
    assert _is_valid_path(grid, path, start, goal)
    assert _path_cost(path) <= 2.0 * optimal_cost + _EPS


def test_weight_equals_one():
    """weight=1.0（通常のA*と等価）でも最適コスト以下であることを確認"""
    grid = [
        [0, 0, 0, 0, 0],
        [0, 1, 1, 1, 0],
        [0, 0, 0, 0, 0],
        [0, 1, 1, 1, 0],
        [0, 0, 0, 0, 0],
    ]
    start, goal = (0, 0), (4, 4)
    path = weighted_astar(grid, start, goal, 1.0)
    assert _is_valid_path(grid, path, start, goal)
    assert _path_cost(path) <= 8.0 + _EPS


def test_start_equals_goal():
    """startとgoalが同一の場合は[start]のみを返す"""
    grid = [[0] * 3 for _ in range(3)]
    path = weighted_astar(grid, (1, 1), (1, 1), 2.0)
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
    assert weighted_astar(grid, (0, 0), (4, 4), 2.0) == []


def test_corner_cut_blocked():
    """斜め移動のコーナーカットが禁止されているため到達不可能になるケース"""
    grid = [
        [0, 1, 0],
        [1, 0, 1],
        [0, 1, 0],
    ]
    assert weighted_astar(grid, (0, 0), (2, 2), 2.0) == []
