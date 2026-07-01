"""rrt_cli.py / rrt_cli_config.pyのテスト（pytest）。

設定読み込み関数を直接呼ぶ単体テスト（正常系・各種異常系）に加え、
subprocess経由で実際にCLIを起動するエンドツーエンドテストを含める。
RRT系は乱数を使うため、E2Eテストはseedを固定して「経路が見つかる」
「start/goal/出力フォーマットが正しい」ことのみ検証する
（厳密な座標一致は検証しない）。
"""
import subprocess
import sys
from pathlib import Path

import pytest

from rrt_cli import main
from rrt_cli_config import parse_rrt_cli_config

CLI_PATH = Path(__file__).resolve().parent / "rrt_cli.py"
EXAMPLE_CONFIG = Path(__file__).resolve().parent / "configs" / "rrt_example.yaml"


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


# --- parse_rrt_cli_config の単体テスト ---


def test_parse_valid_config():
    config = parse_rrt_cli_config(str(EXAMPLE_CONFIG))
    assert config.start == (1.0, 1.0)
    assert config.goal == (9.0, 9.0)
    assert config.bounds == (0.0, 0.0, 10.0, 10.0)
    assert config.seed == 42
    assert config.obstacles == [(5.0, 5.0, 1.0)]


def test_parse_seed_optional(tmp_path):
    path = write_yaml(
        tmp_path, "noseed.yaml",
        "start: [1,1]\ngoal: [9,9]\nbounds: [0,0,10,10]\n"
        "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 100\n",
    )
    config = parse_rrt_cli_config(path)
    assert config.seed is None


def test_parse_file_not_found(tmp_path):
    with pytest.raises(ValueError, match="not found"):
        parse_rrt_cli_config(str(tmp_path / "missing.yaml"))


def test_parse_yaml_syntax_error(tmp_path):
    path = write_yaml(tmp_path, "bad.yaml", "start: [1,1\ngoal: [9,9]\n")
    with pytest.raises(ValueError, match="YAML parse error"):
        parse_rrt_cli_config(path)


def test_parse_missing_field(tmp_path):
    path = write_yaml(tmp_path, "missing.yaml", "start: [1,1]\ngoal: [9,9]\n")
    with pytest.raises(ValueError, match="missing required field"):
        parse_rrt_cli_config(path)


def test_parse_invalid_bounds(tmp_path):
    path = write_yaml(
        tmp_path, "bounds.yaml",
        "start: [1,1]\ngoal: [2,2]\nbounds: [5,0,1,10]\n"
        "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 100\n",
    )
    with pytest.raises(ValueError, match="xmin < xmax"):
        parse_rrt_cli_config(path)


def test_parse_start_outside_bounds(tmp_path):
    path = write_yaml(
        tmp_path, "oob.yaml",
        "start: [-1,-1]\ngoal: [2,2]\nbounds: [0,0,10,10]\n"
        "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 100\n",
    )
    with pytest.raises(ValueError, match="outside of 'bounds'"):
        parse_rrt_cli_config(path)


def test_parse_start_inside_obstacle(tmp_path):
    path = write_yaml(
        tmp_path, "obs.yaml",
        "start: [5,5]\ngoal: [9,9]\nbounds: [0,0,10,10]\n"
        "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 100\n"
        "obstacles:\n  - center: [5,5]\n    radius: 1\n",
    )
    with pytest.raises(ValueError, match="inside an obstacle"):
        parse_rrt_cli_config(path)


def test_parse_non_positive_goal_radius(tmp_path):
    path = write_yaml(
        tmp_path, "radius.yaml",
        "start: [1,1]\ngoal: [9,9]\nbounds: [0,0,10,10]\n"
        "goal_radius: 0\nstep_size: 0.5\nmax_iterations: 100\n",
    )
    with pytest.raises(ValueError, match="goal_radius"):
        parse_rrt_cli_config(path)


def test_parse_non_positive_max_iterations(tmp_path):
    path = write_yaml(
        tmp_path, "iters.yaml",
        "start: [1,1]\ngoal: [9,9]\nbounds: [0,0,10,10]\n"
        "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 0\n",
    )
    with pytest.raises(ValueError, match="max_iterations"):
        parse_rrt_cli_config(path)


def test_parse_non_positive_obstacle_radius(tmp_path):
    path = write_yaml(
        tmp_path, "obsr.yaml",
        "start: [1,1]\ngoal: [9,9]\nbounds: [0,0,10,10]\n"
        "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 100\n"
        "obstacles:\n  - center: [5,5]\n    radius: 0\n",
    )
    with pytest.raises(ValueError, match="radius"):
        parse_rrt_cli_config(path)


# --- main() の単体テスト ---


@pytest.mark.parametrize(
    "algorithm",
    ["rrt", "goal_biased_rrt", "rrt_star", "rrt_connect", "informed_rrt_star"],
)
def test_main_finds_path(algorithm, capsys):
    assert main(["rrt_cli", algorithm, str(EXAMPLE_CONFIG)]) == 0
    out = capsys.readouterr().out.strip()
    assert out.startswith("(1.0,1.0)")


def test_main_unknown_algorithm(capsys):
    assert main(["rrt_cli", "bogus", str(EXAMPLE_CONFIG)]) == 1
    assert "unknown algorithm" in capsys.readouterr().err


def test_main_wrong_arg_count(capsys):
    assert main(["rrt_cli", "rrt"]) == 1
    assert "Usage" in capsys.readouterr().err


# --- エンドツーエンドテスト ---


def test_e2e_rrt_star():
    result = run_cli(["rrt_star", str(EXAMPLE_CONFIG)])
    assert result.returncode == 0
    assert result.stdout.strip().startswith("(1.0,1.0)")


def test_e2e_missing_config_file():
    result = run_cli(["rrt", "/nonexistent/path.yaml"])
    assert result.returncode == 1
    assert "not found" in result.stderr
