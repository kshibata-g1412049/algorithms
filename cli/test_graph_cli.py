"""graph_cli.py / graph_cli_config.pyのテスト（pytest）。

設定読み込み関数を直接呼ぶ単体テスト（正常系・各種異常系）に加え、
subprocess経由で実際にCLIを起動するエンドツーエンドテストを含める。
"""
import subprocess
import sys
from pathlib import Path

import pytest

from graph_cli import main
from graph_cli_config import parse_graph_cli_config

CLI_PATH = Path(__file__).resolve().parent / "graph_cli.py"
EXAMPLE_CONFIG = Path(__file__).resolve().parent / "configs" / "graph_example.yaml"


def run_cli(args):
    return subprocess.run(
        [sys.executable, str(CLI_PATH), *args],
        capture_output=True,
        text=True,
    )


def write_yaml(tmp_path, name, content):
    path = tmp_path / name
    path.write_text(content)
    return str(path)


# --- parse_graph_cli_config の単体テスト ---


def test_parse_valid_config():
    config = parse_graph_cli_config(str(EXAMPLE_CONFIG))
    assert config.start == 0
    assert config.adjacency == [[1, 2], [0, 3], [0, 3], [1, 2]]


def test_parse_file_not_found(tmp_path):
    with pytest.raises(ValueError, match="not found"):
        parse_graph_cli_config(str(tmp_path / "missing.yaml"))


def test_parse_yaml_syntax_error(tmp_path):
    path = write_yaml(tmp_path, "bad.yaml", "start: 0\nadjacency: [1,2\n")
    with pytest.raises(ValueError, match="YAML parse error"):
        parse_graph_cli_config(path)


def test_parse_missing_field(tmp_path):
    path = write_yaml(tmp_path, "missing.yaml", "start: 0\n")
    with pytest.raises(ValueError, match="missing required field 'adjacency'"):
        parse_graph_cli_config(path)


def test_parse_empty_adjacency(tmp_path):
    path = write_yaml(tmp_path, "empty.yaml", "start: 0\nadjacency: []\n")
    with pytest.raises(ValueError, match="must not be empty"):
        parse_graph_cli_config(path)


def test_parse_start_out_of_range(tmp_path):
    path = write_yaml(tmp_path, "oor.yaml", "start: 5\nadjacency:\n  - [1]\n  - [0]\n")
    with pytest.raises(ValueError, match="out of range"):
        parse_graph_cli_config(path)


def test_parse_neighbor_out_of_range(tmp_path):
    path = write_yaml(tmp_path, "oor2.yaml", "start: 0\nadjacency:\n  - [5]\n  - [0]\n")
    with pytest.raises(ValueError, match="out-of-range node id"):
        parse_graph_cli_config(path)


def test_parse_not_a_mapping(tmp_path):
    path = write_yaml(tmp_path, "list.yaml", "- 1\n- 2\n")
    with pytest.raises(ValueError, match="YAML mapping"):
        parse_graph_cli_config(path)


# --- main() の単体テスト ---


def test_main_bfs(capsys):
    assert main(["graph_cli", "bfs", str(EXAMPLE_CONFIG)]) == 0
    assert capsys.readouterr().out.strip() == "0 1 2 3"


def test_main_dfs(capsys):
    assert main(["graph_cli", "dfs", str(EXAMPLE_CONFIG)]) == 0
    assert capsys.readouterr().out.strip() == "0 1 3 2"


def test_main_unknown_algorithm(capsys):
    assert main(["graph_cli", "bogus", str(EXAMPLE_CONFIG)]) == 1
    assert "unknown algorithm" in capsys.readouterr().err


def test_main_wrong_arg_count(capsys):
    assert main(["graph_cli", "bfs"]) == 1
    assert "Usage" in capsys.readouterr().err


# --- エンドツーエンドテスト ---


def test_e2e_bfs():
    result = run_cli(["bfs", str(EXAMPLE_CONFIG)])
    assert result.returncode == 0
    assert result.stdout.strip() == "0 1 2 3"


def test_e2e_missing_config_file():
    result = run_cli(["bfs", "/nonexistent/path.yaml"])
    assert result.returncode == 1
    assert "not found" in result.stderr
