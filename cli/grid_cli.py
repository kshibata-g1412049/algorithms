"""grid_cli: Dijkstra/A*系6種をコマンドラインから試すためのサンプルプログラム。

グリッド・start・goal・weightはCLI引数では表現しにくいため、YAML設定
ファイルのパスを引数で受け取って読み込む方式にしている
（フォーマットはgrid_cli_config.pyを参照）。

使い方: python cli/grid_cli.py <algorithm> <config.yaml>
  例:   python cli/grid_cli.py astar cli/configs/grid_example.yaml

<algorithm> には dijkstra, astar, weighted_astar, bidirectional_astar, jps,
ida_star のいずれかを指定する。
"""
import importlib
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

from grid_cli_config import parse_grid_cli_config  # noqa: E402

ALGORITHMS = {
    "dijkstra": "dijkstra",
    "astar": "astar",
    "weighted_astar": "weighted_astar",
    "bidirectional_astar": "bidirectional_astar",
    "jps": "jump_point_search",
    "ida_star": "ida_star",
}


def print_usage(program_name: str) -> None:
    print(
        f"Usage: {program_name} <algorithm> <config.yaml>\n"
        f"  algorithm: {' | '.join(ALGORITHMS)}\n"
        f"  example:   {program_name} astar cli/configs/grid_example.yaml",
        file=sys.stderr,
    )


def load_grid_function(algorithm: str):
    module_name = ALGORITHMS[algorithm]
    module_dir = REPO_ROOT / "searching" / module_name / "python"
    sys.path.insert(0, str(module_dir))
    module = importlib.import_module(module_name)
    return getattr(module, module_name)


def main(argv: list) -> int:
    if len(argv) != 3:
        print_usage(argv[0])
        return 1

    algorithm = argv[1]
    config_path = argv[2]

    if algorithm not in ALGORITHMS:
        print(f"Error: unknown algorithm '{algorithm}'", file=sys.stderr)
        print_usage(argv[0])
        return 1

    try:
        config = parse_grid_cli_config(config_path)
        grid_function = load_grid_function(algorithm)
        if algorithm == "weighted_astar":
            path = grid_function(config.grid, config.start, config.goal, config.weight)
        else:
            path = grid_function(config.grid, config.start, config.goal)
    except ValueError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    if not path:
        print("no path found")
    else:
        print(" ".join(f"({r},{c})" for r, c in path))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
