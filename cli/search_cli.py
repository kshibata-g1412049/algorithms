"""search_cli: 配列探索7種をコマンドラインから試すためのサンプルプログラム。

各アルゴリズムの実装は searching/<algo>/python/<algo>.py をそのままimportして
再利用する（重複コードなし）。importlibでモジュールを動的にロードするため、
sys.pathの一時的な操作のみで済み、パッケージ化は不要。

使い方: python cli/search_cli.py <algorithm> --target N <numbers...>
  例:   python cli/search_cli.py binary --target 5 1 3 5 7 9

<algorithm> には linear, binary, jump, interpolation, exponential, ternary,
fibonacci のいずれかを指定する。linear以外は入力配列が昇順ソート済みである
ことを前提とするため、本CLIで事前に検証しエラーにする。
"""
import importlib
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent

ALGORITHMS = {
    "linear": "linear_search",
    "binary": "binary_search",
    "jump": "jump_search",
    "interpolation": "interpolation_search",
    "exponential": "exponential_search",
    "ternary": "ternary_search",
    "fibonacci": "fibonacci_search",
}


def print_usage(program_name: str) -> None:
    print(
        f"Usage: {program_name} <algorithm> --target N <numbers...>\n"
        f"  algorithm: {' | '.join(ALGORITHMS)}\n"
        f"  example:   {program_name} binary --target 5 1 3 5 7 9",
        file=sys.stderr,
    )


def load_search_function(algorithm: str):
    """searching/<algorithm>_search/python/ から <algorithm>_search 関数を動的にimportする。"""
    module_name = ALGORITHMS[algorithm]
    module_dir = REPO_ROOT / "searching" / module_name / "python"
    sys.path.insert(0, str(module_dir))
    module = importlib.import_module(module_name)
    return getattr(module, module_name)


def main(argv: list) -> int:
    if len(argv) < 4:
        print_usage(argv[0])
        return 1

    algorithm = argv[1]
    if algorithm not in ALGORITHMS:
        print(f"Error: unknown algorithm '{algorithm}'", file=sys.stderr)
        print_usage(argv[0])
        return 1

    if argv[2] != "--target":
        print("Error: expected '--target N' as the second argument", file=sys.stderr)
        print_usage(argv[0])
        return 1

    try:
        target = int(argv[3])
        values = [int(v) for v in argv[4:]]
    except ValueError as exc:
        print(f"Error: invalid number ({exc})", file=sys.stderr)
        return 1

    if algorithm != "linear" and values != sorted(values):
        print(
            f"Error: {algorithm} search requires the input array to be sorted "
            "in ascending order",
            file=sys.stderr,
        )
        return 1

    search_function = load_search_function(algorithm)
    index = search_function(values, target)
    print(index)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
