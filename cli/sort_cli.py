"""sort_cli: 8種のソートアルゴリズムをコマンドラインから試すためのサンプルプログラム。

各アルゴリズムの実装は sorting/<algo>/python/<algo>_sort.py をそのままimportして
再利用する（重複コードなし）。importlibでモジュールを動的にロードするため、
sys.pathの一時的な操作のみで済み、パッケージ化は不要。

使い方: python cli/sort_cli.py <algorithm> <numbers...>
  例:   python cli/sort_cli.py bubble 5 3 1 4 2

<algorithm> には bubble, selection, insertion, merge, quick, heap, shell,
counting のいずれかを指定する。countingは非負整数のみ対応。
"""
import importlib
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent

ALGORITHMS = {
    "bubble": "bubble_sort",
    "selection": "selection_sort",
    "insertion": "insertion_sort",
    "merge": "merge_sort",
    "quick": "quick_sort",
    "heap": "heap_sort",
    "shell": "shell_sort",
    "counting": "counting_sort",
}


def print_usage(program_name: str) -> None:
    print(
        f"Usage: {program_name} <algorithm> <numbers...>\n"
        f"  algorithm: {' | '.join(ALGORITHMS)}\n"
        f"  example:   {program_name} bubble 5 3 1 4 2",
        file=sys.stderr,
    )


def load_sort_function(algorithm: str):
    """sorting/<algorithm>_sort/python/ から <algorithm>_sort 関数を動的にimportする。"""
    module_name = ALGORITHMS[algorithm]
    module_dir = REPO_ROOT / "sorting" / module_name / "python"
    sys.path.insert(0, str(module_dir))
    module = importlib.import_module(module_name)
    return getattr(module, module_name)


def main(argv: list) -> int:
    if len(argv) < 2:
        print_usage(argv[0])
        return 1

    algorithm = argv[1]
    if algorithm not in ALGORITHMS:
        print(f"Error: unknown algorithm '{algorithm}'", file=sys.stderr)
        print_usage(argv[0])
        return 1

    try:
        values = [int(v) for v in argv[2:]]
    except ValueError as exc:
        print(f"Error: invalid number ({exc})", file=sys.stderr)
        return 1

    if algorithm == "counting" and any(v < 0 for v in values):
        print(
            "Error: counting sort only supports non-negative integers",
            file=sys.stderr,
        )
        return 1

    sort_function = load_sort_function(algorithm)
    result = sort_function(values)
    print(" ".join(str(v) for v in result))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
