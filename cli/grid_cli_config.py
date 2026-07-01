"""grid_cli用のYAML設定ファイル読み込み・検証ロジック。

想定フォーマット:
    start: [0, 0]
    goal: [4, 4]
    weight: 1.5            # weighted_astar以外では無視される（省略可、既定1.0）
    grid:
      - [0, 0, 0, 0, 0]
      - [0, 1, 1, 1, 0]
      - [0, 0, 0, 0, 0]
      - [0, 1, 1, 1, 0]
      - [0, 0, 0, 0, 0]

grid: 0=通行可, 1=障害物の2次元配列（矩形であること）
start/goal: [row, col]（0始まり、グリッド範囲内かつ障害物でないセル）
weight: weighted_astarのヒューリスティック重み（1.0以上）

ファイル未存在・YAML構文エラー・必須フィールド欠落・型不正・グリッドが
矩形でない・start/goalが範囲外or障害物上・weightが1.0未満、といった
ケースはすべて ValueError として分かりやすいメッセージ付きで送出する。
"""
from dataclasses import dataclass
from typing import List, Tuple

import yaml


@dataclass
class GridCliConfig:
    grid: List[List[int]]
    start: Tuple[int, int]
    goal: Tuple[int, int]
    weight: float = 1.0


def _as_int(value, field_name: str, path: str) -> int:
    if isinstance(value, bool) or not isinstance(value, int):
        raise ValueError(f"field '{field_name}' must be an integer in {path}")
    return value


def _as_number(value, field_name: str, path: str) -> float:
    if isinstance(value, bool) or not isinstance(value, (int, float)):
        raise ValueError(f"field '{field_name}' must be a number in {path}")
    return float(value)


def _parse_point(value, field_name: str, path: str) -> Tuple[int, int]:
    if not isinstance(value, list) or len(value) != 2:
        raise ValueError(
            f"field '{field_name}' must be a 2-element list [row, col] in {path}"
        )
    return (_as_int(value[0], field_name, path), _as_int(value[1], field_name, path))


def parse_grid_cli_config(path: str) -> GridCliConfig:
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

    for field in ("start", "goal", "grid"):
        if field not in root:
            raise ValueError(f"missing required field '{field}' in {path}")

    grid_raw = root["grid"]
    if not isinstance(grid_raw, list) or len(grid_raw) == 0:
        raise ValueError(f"field 'grid' must be a non-empty list of lists in {path}")

    grid: List[List[int]] = []
    num_cols = None
    for row in grid_raw:
        if not isinstance(row, list):
            raise ValueError(f"each entry in 'grid' must be a list in {path}")
        if num_cols is None:
            num_cols = len(row)
        elif len(row) != num_cols:
            raise ValueError(f"field 'grid' must be rectangular in {path}")
        parsed_row = []
        for cell in row:
            cell_int = _as_int(cell, "grid", path)
            if cell_int not in (0, 1):
                raise ValueError(f"field 'grid' cells must be 0 or 1 in {path}")
            parsed_row.append(cell_int)
        grid.append(parsed_row)

    if num_cols == 0:
        raise ValueError(f"field 'grid' must be a non-empty list of lists in {path}")

    start = _parse_point(root["start"], "start", path)
    goal = _parse_point(root["goal"], "goal", path)

    num_rows = len(grid)

    def check_in_bounds_and_free(point: Tuple[int, int], field_name: str) -> None:
        row, col = point
        if row < 0 or row >= num_rows or col < 0 or col >= num_cols:
            raise ValueError(f"field '{field_name}' is outside of 'grid' bounds in {path}")
        if grid[row][col] != 0:
            raise ValueError(f"field '{field_name}' is on an obstacle cell in {path}")

    check_in_bounds_and_free(start, "start")
    check_in_bounds_and_free(goal, "goal")

    weight = 1.0
    if "weight" in root:
        weight = _as_number(root["weight"], "weight", path)
        if weight < 1.0:
            raise ValueError(f"field 'weight' must be >= 1.0 in {path}")

    return GridCliConfig(grid=grid, start=start, goal=goal, weight=weight)
