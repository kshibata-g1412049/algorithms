"""graph_cli用のYAML設定ファイル読み込み・検証ロジック。

想定フォーマット:
    start: 0
    adjacency:
      - [1, 2]
      - [0, 3]
      - [0, 3]
      - [1, 2]

start: 探索開始ノード（0始まり）
adjacency: 0始まりの隣接リスト（adjacency[i]がノードiの隣接ノード番号のリスト）

ファイル未存在・YAML構文エラー・必須フィールド欠落・型不正・ノード番号が
範囲外、といったケースはすべて ValueError として分かりやすいメッセージ付きで
送出する。
"""
from dataclasses import dataclass
from typing import List

import yaml


@dataclass
class GraphCliConfig:
    start: int
    adjacency: List[List[int]]


def _as_int(value, field_name: str, path: str) -> int:
    if isinstance(value, bool) or not isinstance(value, int):
        raise ValueError(f"field '{field_name}' must be an integer in {path}")
    return value


def parse_graph_cli_config(path: str) -> GraphCliConfig:
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

    if "start" not in root:
        raise ValueError(f"missing required field 'start' in {path}")
    if "adjacency" not in root:
        raise ValueError(f"missing required field 'adjacency' in {path}")

    start = _as_int(root["start"], "start", path)

    adjacency_raw = root["adjacency"]
    if not isinstance(adjacency_raw, list):
        raise ValueError(f"field 'adjacency' must be a list of lists in {path}")

    adjacency: List[List[int]] = []
    for row in adjacency_raw:
        if not isinstance(row, list):
            raise ValueError(f"each entry in 'adjacency' must be a list in {path}")
        neighbors = [_as_int(v, "adjacency", path) for v in row]
        adjacency.append(neighbors)

    num_nodes = len(adjacency)
    if num_nodes == 0:
        raise ValueError(f"'adjacency' must not be empty in {path}")

    if start < 0 or start >= num_nodes:
        raise ValueError(
            f"'start' ({start}) is out of range [0, {num_nodes}) in {path}"
        )

    for neighbors in adjacency:
        for neighbor in neighbors:
            if neighbor < 0 or neighbor >= num_nodes:
                raise ValueError(
                    f"'adjacency' contains out-of-range node id {neighbor} in {path}"
                )

    return GraphCliConfig(start=start, adjacency=adjacency)
