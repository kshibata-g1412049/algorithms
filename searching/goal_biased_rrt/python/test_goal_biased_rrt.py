"""goal_biased_rrt関数のテスト（pytest）。

このファイルは同じディレクトリ内のgoal_biased_rrt.pyのみに依存しているため、
python/フォルダを単独で取り出してもこのテストは実行できる。

Goal-biased RRTは乱数を使うアルゴリズムであり、固定シードでもC++/Python/MATLAB
間で同じ乱数列にはならない（言語ごとに乱数生成器の実装が異なるため）。
そのため、ここでは出力の厳密な値ではなく、以下の「性質」を検証する
プロパティベーステストを行う:
  1. 十分大きなmax_iterationsであれば、複数の固定シードで経路が見つかる。
  2. 経路の先頭がstartと一致する。
  3. 経路の末尾がgoalからgoal_radius以内にある。
  4. 経路上の各線分が障害物と衝突していない。
  5. 経路上の各ステップの移動距離がstep_size以下（許容誤差込み）である。
  6. 障害物による迂回が必要なシナリオでも1〜5が成り立つ。
  7. max_iterationsが極端に小さい場合は空、または性質を満たす経路のみが返る。
"""
import math

import pytest

from goal_biased_rrt import goal_biased_rrt

BOUNDS = (0.0, 0.0, 10.0, 10.0)
START = (1.0, 1.0)
GOAL = (9.0, 9.0)
GOAL_RADIUS = 0.5
STEP_SIZE = 0.5
MAX_ITERATIONS = 5000


def _distance(p1, p2):
    return math.hypot(p2[0] - p1[0], p2[1] - p1[1])


def _point_to_segment_distance(p, a, b):
    dx = b[0] - a[0]
    dy = b[1] - a[1]
    length_squared = dx * dx + dy * dy
    if length_squared < 1e-12:
        return _distance(p, a)
    t = ((p[0] - a[0]) * dx + (p[1] - a[1]) * dy) / length_squared
    t = max(0.0, min(1.0, t))
    closest = (a[0] + t * dx, a[1] + t * dy)
    return _distance(p, closest)


def _assert_valid_path(path, start, goal, goal_radius, step_size, obstacles):
    """経路が満たすべき性質（2〜5）をまとめて検証するヘルパー。"""
    assert path, "path should not be empty"

    # 2. 先頭がstartと一致する
    assert path[0] == pytest.approx(start, abs=1e-9)

    # 3. 末尾がgoalからgoal_radius以内にある
    assert _distance(path[-1], goal) <= goal_radius + 1e-6

    # 4, 5. 各線分が障害物と衝突していない & 各ステップがstep_size以下
    for i in range(len(path) - 1):
        p1, p2 = path[i], path[i + 1]
        step_dist = _distance(p1, p2)
        assert step_dist <= step_size * 1.01, (
            f"step {i} distance {step_dist} exceeds step_size*1.01"
        )
        for cx, cy, radius in obstacles:
            d = _point_to_segment_distance((cx, cy), p1, p2)
            assert d > radius, f"segment {i} collides with obstacle at ({cx}, {cy})"


@pytest.mark.parametrize("seed", [1, 2, 3])
def test_no_obstacles_finds_path(seed):
    """障害物なしのシンプルなシナリオで、複数の固定シードで経路が見つかる。"""
    obstacles = []
    path = goal_biased_rrt(obstacles, BOUNDS, START, GOAL, GOAL_RADIUS, STEP_SIZE,
                           MAX_ITERATIONS, seed)
    _assert_valid_path(path, START, GOAL, GOAL_RADIUS, STEP_SIZE, obstacles)


@pytest.mark.parametrize("seed", [1, 2, 3])
def test_with_obstacle_requires_detour(seed):
    """start-goal間に大きな円障害物を配置し、迂回が必要なシナリオでも性質が成り立つ。"""
    obstacles = [(5.0, 5.0, 2.0)]
    path = goal_biased_rrt(obstacles, BOUNDS, START, GOAL, GOAL_RADIUS, STEP_SIZE,
                           MAX_ITERATIONS, seed)
    _assert_valid_path(path, START, GOAL, GOAL_RADIUS, STEP_SIZE, obstacles)


def test_tiny_max_iterations_returns_empty_or_valid():
    """max_iterationsが極端に小さい場合、空リストか性質を満たす経路のみが返る。"""
    obstacles = []
    path = goal_biased_rrt(obstacles, BOUNDS, START, GOAL, GOAL_RADIUS, STEP_SIZE, 1, 42)
    if path:
        _assert_valid_path(path, START, GOAL, GOAL_RADIUS, STEP_SIZE, obstacles)
    else:
        assert path == []


def test_seed_reproducibility():
    """同じシードであれば同じ経路が再現される（Python内での再現性確認）。"""
    obstacles = []
    path1 = goal_biased_rrt(obstacles, BOUNDS, START, GOAL, GOAL_RADIUS, STEP_SIZE,
                            MAX_ITERATIONS, 7)
    path2 = goal_biased_rrt(obstacles, BOUNDS, START, GOAL, GOAL_RADIUS, STEP_SIZE,
                            MAX_ITERATIONS, 7)
    assert path1 == path2
