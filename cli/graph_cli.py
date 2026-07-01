"""graph_cli: BFS/DFSをコマンドラインから試すためのサンプルプログラム。

グラフ（隣接リスト）はCLI引数では表現しにくいため、YAML設定ファイルの
パスを引数で受け取って読み込む方式にしている
（フォーマットはgraph_cli_config.pyを参照）。

使い方: python cli/graph_cli.py <algorithm> <config.yaml>
  例:   python cli/graph_cli.py bfs cli/configs/graph_example.yaml

<algorithm> には bfs, dfs のいずれかを指定する。
"""
import importlib
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

from graph_cli_config import parse_graph_cli_config  # noqa: E402

ALGORITHMS = {
    "bfs": "bfs",
    "dfs": "dfs",
}


def print_usage(program_name: str) -> None:
    print(
        f"Usage: {program_name} <algorithm> <config.yaml>\n"
        f"  algorithm: {' | '.join(ALGORITHMS)}\n"
        f"  example:   {program_name} bfs cli/configs/graph_example.yaml",
        file=sys.stderr,
    )


def load_graph_function(algorithm: str):
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
        config = parse_graph_cli_config(config_path)
        graph_function = load_graph_function(algorithm)
        order = graph_function(config.adjacency, config.start)
    except ValueError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    print(" ".join(str(v) for v in order))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
