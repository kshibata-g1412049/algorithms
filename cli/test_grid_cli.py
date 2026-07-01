"""grid_cli.py / grid_cli_config.pyのテスト（pytest）。

設定読み込み関数を直接呼ぶ単体テスト（正常系・各種異常系）に加え、
subprocess経由で実際にCLIを起動するエンドツーエンドテストを含める。
"""
import subprocess
import sys
from pathlib import Path

import pytest

from grid_cli import main
from grid_cli_config import parse_grid_cli_config

CLI_PATH = Path(__file__).resolve().parent / "grid_cli.py"
EXAMPLE_CONFIG = Path(__file__).resolve().parent / "configs" / "grid_example.yaml"


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


# --- parse_grid_cli_config の単体テスト ---


def test_parse_valid_config():
    config = parse_grid_cli_config(str(EXAMPLE_CONFIG))
    assert config.start == (0, 0)
    assert config.goal == (4, 4)
    assert config.weight == 1.5
    assert len(config.grid) == 5


def test_parse_default_weight(tmp_path):
    path = write_yaml(
        tmp_path, "noweight.yaml",
        "start: [0,0]\ngoal: [1,1]\ngrid:\n  - [0,0]\n  - [0,0]\n",
    )
    config = parse_grid_cli_config(path)
    assert config.weight == 1.0


def test_parse_file_not_found(tmp_path):
    with pytest.raises(ValueError, match="not found"):
        parse_grid_cli_config(str(tmp_path / "missing.yaml"))


def test_parse_yaml_syntax_error(tmp_path):
    path = write_yaml(tmp_path, "bad.yaml", "start: [0,0\ngoal: [1,1]\n")
    with pytest.raises(ValueError, match="YAML parse error"):
        parse_grid_cli_config(path)


def test_parse_missing_field(tmp_path):
    path = write_yaml(tmp_path, "missing.yaml", "start: [0,0]\ngoal: [1,1]\n")
    with pytest.raises(ValueError, match="missing required field 'grid'"):
        parse_grid_cli_config(path)


def test_parse_non_rectangular_grid(tmp_path):
    path = write_yaml(
        tmp_path, "bad_shape.yaml",
        "start: [0,0]\ngoal: [1,1]\ngrid:\n  - [0,0]\n  - [0]\n",
    )
    with pytest.raises(ValueError, match="rectangular"):
        parse_grid_cli_config(path)


def test_parse_invalid_cell_value(tmp_path):
    path = write_yaml(
        tmp_path, "bad_cell.yaml",
        "start: [0,0]\ngoal: [1,1]\ngrid:\n  - [0,2]\n  - [0,0]\n",
    )
    with pytest.raises(ValueError, match="0 or 1"):
        parse_grid_cli_config(path)


def test_parse_start_out_of_bounds(tmp_path):
    path = write_yaml(
        tmp_path, "oob.yaml",
        "start: [5,5]\ngoal: [0,0]\ngrid:\n  - [0,0]\n  - [0,0]\n",
    )
    with pytest.raises(ValueError, match="outside of 'grid' bounds"):
        parse_grid_cli_config(path)


def test_parse_start_on_obstacle(tmp_path):
    path = write_yaml(
        tmp_path, "onobs.yaml",
        "start: [1,1]\ngoal: [0,0]\ngrid:\n  - [0,0]\n  - [0,1]\n",
    )
    with pytest.raises(ValueError, match="obstacle cell"):
        parse_grid_cli_config(path)


def test_parse_weight_too_small(tmp_path):
    path = write_yaml(
        tmp_path, "weight.yaml",
        "start: [0,0]\ngoal: [1,1]\nweight: 0.5\ngrid:\n  - [0,0]\n  - [0,0]\n",
    )
    with pytest.raises(ValueError, match="weight"):
        parse_grid_cli_config(path)


# --- main() の単体テスト ---


@pytest.mark.parametrize(
    "algorithm",
    ["dijkstra", "astar", "weighted_astar", "bidirectional_astar", "jps", "ida_star"],
)
def test_main_finds_path(algorithm, capsys):
    assert main(["grid_cli", algorithm, str(EXAMPLE_CONFIG)]) == 0
    out = capsys.readouterr().out.strip()
    assert out.startswith("(0,0)")
    assert out.endswith("(4,4)")


def test_main_unknown_algorithm(capsys):
    assert main(["grid_cli", "bogus", str(EXAMPLE_CONFIG)]) == 1
    assert "unknown algorithm" in capsys.readouterr().err


def test_main_wrong_arg_count(capsys):
    assert main(["grid_cli", "astar"]) == 1
    assert "Usage" in capsys.readouterr().err


# --- エンドツーエンドテスト ---


def test_e2e_astar():
    result = run_cli(["astar", str(EXAMPLE_CONFIG)])
    assert result.returncode == 0
    assert result.stdout.strip().startswith("(0,0)")


def test_e2e_missing_config_file():
    result = run_cli(["astar", "/nonexistent/path.yaml"])
    assert result.returncode == 1
    assert "not found" in result.stderr
