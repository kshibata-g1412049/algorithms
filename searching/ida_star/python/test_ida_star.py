"""ida_star関数のテスト（pytest）。

このファイルは同じディレクトリ内のida_star.pyのみに依存して
いるため、python/フォルダを単独で取り出してもこのテストは実行できる。
"""
import math

from ida_star import ida_star

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
    grid = [[0] * 5 for _ in range(5)]
    start, goal = (0, 0), (4, 4)
    path = ida_star(grid, start, goal)
    assert _is_valid_path(grid, path, start, goal)
    assert abs(_path_cost(path) - 4 * _SQRT2) < _EPS


def test_wall_with_gap_detour():
    grid = [
        [0, 0, 0, 0, 0],
        [0, 1, 1, 1, 0],
        [0, 0, 0, 0, 0],
        [0, 1, 1, 1, 0],
        [0, 0, 0, 0, 0],
    ]
    start, goal = (0, 0), (4, 4)
    path = ida_star(grid, start, goal)
    assert _is_valid_path(grid, path, start, goal)
    assert abs(_path_cost(path) - 8.0) < _EPS


def test_start_equals_goal():
    grid = [[0] * 3 for _ in range(3)]
    path = ida_star(grid, (1, 1), (1, 1))
    assert path == [(1, 1)]


def test_unreachable_goal():
    grid = [
        [0, 0, 0, 0, 0],
        [1, 1, 1, 1, 1],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0],
        [0, 0, 0, 0, 0],
    ]
    assert ida_star(grid, (0, 0), (4, 4)) == []


def test_corner_cut_blocked():
    grid = [
        [0, 1, 0],
        [1, 0, 1],
        [0, 1, 0],
    ]
    assert ida_star(grid, (0, 0), (2, 2)) == []


def test_complex_grid():
    """期待コスト13は独立実装のDijkstraで事前に計算・確認済み。

    IDA*はclosedリストを持たないため最悪ケースでは指数時間となるが、
    このサイズのグリッド（8x7）では実測で数百ノード以下の探索で
    即座に解が見つかることを確認済み。
    """
    grid = [
        [0, 0, 0, 0, 0, 0, 0, 0],
        [0, 1, 1, 0, 1, 1, 1, 0],
        [0, 1, 0, 0, 0, 0, 1, 0],
        [0, 1, 0, 1, 1, 0, 1, 0],
        [0, 0, 0, 1, 0, 0, 0, 0],
        [0, 1, 1, 1, 0, 1, 1, 0],
        [0, 0, 0, 0, 0, 0, 0, 0],
    ]
    start, goal = (0, 0), (6, 7)
    path = ida_star(grid, start, goal)
    assert _is_valid_path(grid, path, start, goal)
    assert abs(_path_cost(path) - 13.0) < _EPS


def test_unforced_diagonal_shortcut():
    """コーナーカット禁止下では、近くに障害物が一つもない地点でも、最短経路が
    「強制されていない」斜めの折れを必要とすることがある（地点(1,2)で、直進中に
    斜めへ折れないと最短コスト12+sqrt(2)を達成できない）。"""
    grid = [
        [0, 0, 1, 1, 1, 0],
        [0, 0, 0, 0, 0, 1],
        [0, 1, 0, 0, 0, 0],
        [1, 1, 1, 0, 0, 1],
        [0, 1, 0, 0, 1, 1],
        [0, 1, 0, 0, 0, 0],
        [1, 0, 1, 0, 1, 1],
        [0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 1, 0, 1, 0],
    ]
    start, goal = (0, 1), (8, 0)
    path = ida_star(grid, start, goal)
    assert _is_valid_path(grid, path, start, goal)
    assert abs(_path_cost(path) - (11.0 + _SQRT2)) < _EPS
