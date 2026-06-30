"""rrt_cli用のYAML設定ファイル読み込み・検証ロジック。

想定フォーマット:
    start: [1.0, 1.0]
    goal: [9.0, 9.0]
    bounds: [0.0, 0.0, 10.0, 10.0]   # xmin, ymin, xmax, ymax
    goal_radius: 0.5
    step_size: 0.5
    max_iterations: 5000
    seed: 42                          # 省略可（省略時はランダムなシードを使用）
    obstacles:
      - center: [5.0, 5.0]
        radius: 1.0

ファイル未存在・YAML構文エラー・必須フィールド欠落・型不正・boundsが
xmin<xmax/ymin<ymaxを満たさない・goal_radius/step_size/max_iterations/
障害物半径が0以下・start/goalがbounds外or障害物内、といったケースは
すべて ValueError として分かりやすいメッセージ付きで送出する。
"""
from dataclasses import dataclass, field
from typing import List, Optional, Tuple

import yaml


@dataclass
class RrtCliConfig:
    start: Tuple[float, float]
    goal: Tuple[float, float]
    bounds: Tuple[float, float, float, float]
    goal_radius: float
    step_size: float
    max_iterations: int
    obstacles: List[Tuple[float, float, float]] = field(default_factory=list)
    seed: Optional[int] = None


def _as_number(value, field_name: str, path: str) -> float:
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ValueError(f"field '{field_name}' must be a number in {path}")
    return float(value)


def _parse_point(value, field_name: str, path: str) -> Tuple[float, float]:
    if not isinstance(value, list) or len(value) != 2:
        raise ValueError(
            f"field '{field_name}' must be a 2-element list [x, y] in {path}"
        )
    return (
        _as_number(value[0], field_name, path),
        _as_number(value[1], field_name, path),
    )


def _point_in_bounds(point: Tuple[float, float], bounds: Tuple[float, float, float, float]) -> bool:
    x, y = point
    xmin, ymin, xmax, ymax = bounds
    return xmin <= x <= xmax and ymin <= y <= ymax


def _point_inside_obstacle(
    point: Tuple[float, float], obstacles: List[Tuple[float, float, float]]
) -> bool:
    x, y = point
    for cx, cy, radius in obstacles:
        dx = x - cx
        dy = y - cy
        if dx * dx + dy * dy < radius * radius:
            return True
    return False


def parse_rrt_cli_config(path: str) -> RrtCliConfig:
    try:
        with open(path, "r", encoding="utf-8") as f:
            try:
                root = yaml.safe_load(f)
            except yaml.YAMLError as exc:
                raise ValueError(f"YAML parse error in {path}: {exc}") from exc
    except FileNotFoundError as exc:
        raise ValueError(f"config file not found or not readable: {path}") from exc
    except OSError as exc:
        raise ValueError(f"config file not found or not readable: {path}") from exc

    if not isinstance(root, dict):
        raise ValueError(f"config file must contain a YAML mapping: {path}")

    for field_name in ("start", "goal", "bounds", "goal_radius", "step_size", "max_iterations"):
        if field_name not in root:
            raise ValueError(f"missing required field '{field_name}' in {path}")

    bounds_raw = root["bounds"]
    if not isinstance(bounds_raw, list) or len(bounds_raw) != 4:
        raise ValueError(
            f"field 'bounds' must be a 4-element list [xmin, ymin, xmax, ymax] in {path}"
        )
    bounds = tuple(_as_number(v, "bounds", path) for v in bounds_raw)
    if bounds[0] >= bounds[2] or bounds[1] >= bounds[3]:
        raise ValueError(f"field 'bounds' must satisfy xmin < xmax and ymin < ymax in {path}")

    start = _parse_point(root["start"], "start", path)
    goal = _parse_point(root["goal"], "goal", path)

    goal_radius = _as_number(root["goal_radius"], "goal_radius", path)
    if goal_radius <= 0.0:
        raise ValueError(f"field 'goal_radius' must be > 0 in {path}")

    step_size = _as_number(root["step_size"], "step_size", path)
    if step_size <= 0.0:
        raise ValueError(f"field 'step_size' must be > 0 in {path}")

    max_iterations_raw = root["max_iterations"]
    if isinstance(max_iterations_raw, bool) or not isinstance(max_iterations_raw, int):
        raise ValueError(f"field 'max_iterations' must be an integer in {path}")
    max_iterations = max_iterations_raw
    if max_iterations <= 0:
        raise ValueError(f"field 'max_iterations' must be > 0 in {path}")

    obstacles: List[Tuple[float, float, float]] = []
    if "obstacles" in root:
        obstacles_raw = root["obstacles"]
        if not isinstance(obstacles_raw, list):
            raise ValueError(f"field 'obstacles' must be a list in {path}")
        for obstacle_node in obstacles_raw:
            if not isinstance(obstacle_node, dict) or "center" not in obstacle_node or "radius" not in obstacle_node:
                raise ValueError(
                    f"each entry in 'obstacles' must have 'center' and 'radius' in {path}"
                )
            cx, cy = _parse_point(obstacle_node["center"], "obstacles.center", path)
            radius = _as_number(obstacle_node["radius"], "obstacles.radius", path)
            if radius <= 0.0:
                raise ValueError(f"'obstacles' radius must be > 0 in {path}")
            obstacles.append((cx, cy, radius))

    if not _point_in_bounds(start, bounds):
        raise ValueError(f"'start' is outside of 'bounds' in {path}")
    if not _point_in_bounds(goal, bounds):
        raise ValueError(f"'goal' is outside of 'bounds' in {path}")
    if _point_inside_obstacle(start, obstacles):
        raise ValueError(f"'start' is inside an obstacle in {path}")
    if _point_inside_obstacle(goal, obstacles):
        raise ValueError(f"'goal' is inside an obstacle in {path}")

    seed: Optional[int] = None
    if "seed" in root:
        seed_raw = root["seed"]
        if isinstance(seed_raw, bool) or not isinstance(seed_raw, int) or seed_raw < 0:
            raise ValueError(f"field 'seed' must be a non-negative integer in {path}")
        seed = seed_raw

    return RrtCliConfig(
        start=start,
        goal=goal,
        bounds=bounds,
        goal_radius=goal_radius,
        step_size=step_size,
        max_iterations=max_iterations,
        obstacles=obstacles,
        seed=seed,
    )
