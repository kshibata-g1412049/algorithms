"""search_cli.pyのテスト（pytest）。

main()を直接呼び出す単体テストに加え、subprocess経由で実際にCLIを起動する
エンドツーエンドテストも含める。
"""
import subprocess
import sys
from pathlib import Path

from search_cli import main

CLI_PATH = Path(__file__).resolve().parent / "search_cli.py"


def run_cli(args):
    return subprocess.run(
        [sys.executable, str(CLI_PATH), *args],
        capture_output=True,
        text=True,
    )


def test_binary_search_found(capsys):
    assert main(["search_cli", "binary", "--target", "5", "1", "3", "5", "7", "9"]) == 0
    assert capsys.readouterr().out.strip() == "2"


def test_linear_search_not_found(capsys):
    assert main(["search_cli", "linear", "--target", "99", "1", "2", "3"]) == 0
    assert capsys.readouterr().out.strip() == "-1"


def test_unknown_algorithm(capsys):
    assert main(["search_cli", "bogus", "--target", "1", "1", "2"]) == 1
    assert "unknown algorithm" in capsys.readouterr().err


def test_missing_target_flag(capsys):
    assert main(["search_cli", "binary", "5", "1", "2", "3"]) == 1
    assert "--target" in capsys.readouterr().err


def test_unsorted_input_rejected_for_binary(capsys):
    assert main(["search_cli", "binary", "--target", "5", "9", "3", "1"]) == 1
    assert "sorted" in capsys.readouterr().err


def test_invalid_number(capsys):
    assert main(["search_cli", "linear", "--target", "x", "1", "2"]) == 1
    assert "invalid number" in capsys.readouterr().err


def test_too_few_args(capsys):
    assert main(["search_cli"]) == 1
    assert "Usage" in capsys.readouterr().err


def test_e2e_binary_search():
    result = run_cli(["binary", "--target", "5", "1", "3", "5", "7", "9"])
    assert result.returncode == 0
    assert result.stdout.strip() == "2"


def test_e2e_unsorted_error():
    result = run_cli(["binary", "--target", "5", "9", "3", "1"])
    assert result.returncode == 1
    assert "sorted" in result.stderr
