"""rrt_cli: RRT系5種をコマンドラインから試すためのサンプルプログラム。

探索領域・障害物・start/goal・各種パラメータは単純なCLI引数では表現しにくい
ため、YAML設定ファイルのパスを引数で受け取って読み込む方式にしている
（フォーマットはrrt_cli_config.pyを参照）。

使い方: python cli/rrt_cli.py <algorithm> <config.yaml>
  例:   python cli/rrt_cli.py rrt_star cli/configs/rrt_example.yaml

<algorithm> には rrt, goal_biased_rrt, rrt_star, rrt_connect,
informed_rrt_star のいずれかを指定する。configでseedを省略した場合は
random.SystemRandomによるランダムなシードを使用する（実行のたびに結果が
変わる）。
"""
import importlib
import random
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

from rrt_cli_config import parse_rrt_cli_config  # noqa: E402

ALGORITHMS = {
    "rrt": "rrt",
    "goal_biased_rrt": "goal_biased_rrt",
    "rrt_star": "rrt_star",
    "rrt_connect": "rrt_connect",
    "informed_rrt_star": "informed_rrt_star",
}


def print_usage(program_name: str) -> None:
    print(
        f"Usage: {program_name} <algorithm> <config.yaml>\n"
        f"  algorithm: {' | '.join(ALGORITHMS)}\n"
        f"  example:   {program_name} rrt_star cli/configs/rrt_example.yaml",
        file=sys.stderr,
    )


def load_rrt_function(algorithm: str):
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
        config = parse_rrt_cli_config(config_path)
        seed = config.seed if config.seed is not None else random.SystemRandom().randrange(2**32)
        rrt_function = load_rrt_function(algorithm)
        path = rrt_function(
            config.obstacles,
            config.bounds,
            config.start,
            config.goal,
            config.goal_radius,
            config.step_size,
            config.max_iterations,
            seed,
        )
    except ValueError as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    if not path:
        print("no path found")
    else:
        print(" ".join(f"({x},{y})" for x, y in path))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
