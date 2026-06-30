// rrt_cli_config.cppのテスト。
//
// 正常系（有効なYAML設定の読み込み、seed省略時の挙動）と異常系（ファイル
// 未存在・YAML構文エラー・必須フィールド欠落・bounds不正・start/goalが
// bounds外or障害物内・各種パラメータが0以下）を網羅する。
#include "rrt_cli_config.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

bool allPassed = true;

void reportPass(const std::string& testName) {
    std::cout << "[PASS] " << testName << std::endl;
}

void reportFail(const std::string& testName, const std::string& detail) {
    std::cerr << "[FAIL] " << testName << ": " << detail << std::endl;
    allPassed = false;
}

std::string writeTempFile(const std::string& name, const std::string& content) {
    const std::string path = "/tmp/rrt_cli_config_test_" + name + ".yaml";
    std::ofstream out(path);
    out << content;
    out.close();
    return path;
}

void expectThrowsContaining(const std::string& testName, const std::string& path,
                             const std::string& expectedSubstring) {
    try {
        parseRrtCliConfig(path);
        reportFail(testName, "expected std::runtime_error but none was thrown");
    } catch (const std::runtime_error& e) {
        const std::string message = e.what();
        if (message.find(expectedSubstring) != std::string::npos) {
            reportPass(testName);
        } else {
            reportFail(testName, "message '" + message + "' does not contain '" +
                                      expectedSubstring + "'");
        }
    } catch (...) {
        reportFail(testName, "threw an exception that is not std::runtime_error");
    }
}

}  // namespace

int main() {
    // 正常系: 有効な設定ファイル（obstaclesとseedを含む）
    {
        const std::string path = writeTempFile(
            "valid",
            "start: [1.0, 1.0]\ngoal: [9.0, 9.0]\nbounds: [0.0, 0.0, 10.0, 10.0]\n"
            "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 5000\nseed: 42\n"
            "obstacles:\n  - center: [5.0, 5.0]\n    radius: 1.0\n");
        try {
            const RrtCliConfig config = parseRrtCliConfig(path);
            if (config.start == std::make_pair(1.0, 1.0) &&
                config.goal == std::make_pair(9.0, 9.0) && config.seed.has_value() &&
                config.seed.value() == 42 && config.obstacles.size() == 1) {
                reportPass("valid config");
            } else {
                reportFail("valid config", "parsed values did not match expectations");
            }
        } catch (const std::exception& e) {
            reportFail("valid config", std::string("unexpected exception: ") + e.what());
        }
    }

    // 正常系: seed省略時はstd::nullopt
    {
        const std::string path = writeTempFile(
            "no_seed",
            "start: [1.0, 1.0]\ngoal: [9.0, 9.0]\nbounds: [0.0, 0.0, 10.0, 10.0]\n"
            "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 100\n");
        try {
            const RrtCliConfig config = parseRrtCliConfig(path);
            if (!config.seed.has_value()) {
                reportPass("seed omitted");
            } else {
                reportFail("seed omitted", "seed should be unset");
            }
        } catch (const std::exception& e) {
            reportFail("seed omitted", std::string("unexpected exception: ") + e.what());
        }
    }

    // 異常系: ファイルが存在しない
    expectThrowsContaining("file not found", "/tmp/rrt_cli_config_test_nonexistent.yaml",
                            "not found");

    // 異常系: YAML構文エラー
    {
        const std::string path = writeTempFile("bad_syntax", "start: [1.0, 1.0\ngoal: [9, 9]\n");
        expectThrowsContaining("YAML syntax error", path, "parse error");
    }

    // 異常系: 必須フィールド欠落
    {
        const std::string path = writeTempFile("missing_field", "start: [1.0, 1.0]\ngoal: [9.0, 9.0]\n");
        expectThrowsContaining("missing field 'bounds'", path, "bounds");
    }

    // 異常系: boundsがxmin<xmaxを満たさない
    {
        const std::string path = writeTempFile(
            "bad_bounds",
            "start: [1.0, 1.0]\ngoal: [2.0, 2.0]\nbounds: [5.0, 0.0, 1.0, 10.0]\n"
            "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 100\n");
        expectThrowsContaining("invalid bounds", path, "xmin < xmax");
    }

    // 異常系: startがbounds外
    {
        const std::string path = writeTempFile(
            "start_oob",
            "start: [-1.0, -1.0]\ngoal: [2.0, 2.0]\nbounds: [0.0, 0.0, 10.0, 10.0]\n"
            "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 100\n");
        expectThrowsContaining("start outside bounds", path, "outside of 'bounds'");
    }

    // 異常系: startが障害物内
    {
        const std::string path = writeTempFile(
            "start_in_obs",
            "start: [5.0, 5.0]\ngoal: [9.0, 9.0]\nbounds: [0.0, 0.0, 10.0, 10.0]\n"
            "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 100\n"
            "obstacles:\n  - center: [5.0, 5.0]\n    radius: 1.0\n");
        expectThrowsContaining("start inside obstacle", path, "inside an obstacle");
    }

    // 異常系: goal_radiusが0以下
    {
        const std::string path = writeTempFile(
            "bad_radius",
            "start: [1.0, 1.0]\ngoal: [9.0, 9.0]\nbounds: [0.0, 0.0, 10.0, 10.0]\n"
            "goal_radius: 0\nstep_size: 0.5\nmax_iterations: 100\n");
        expectThrowsContaining("non-positive goal_radius", path, "goal_radius");
    }

    // 異常系: max_iterationsが0以下
    {
        const std::string path = writeTempFile(
            "bad_iterations",
            "start: [1.0, 1.0]\ngoal: [9.0, 9.0]\nbounds: [0.0, 0.0, 10.0, 10.0]\n"
            "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 0\n");
        expectThrowsContaining("non-positive max_iterations", path, "max_iterations");
    }

    // 異常系: 障害物半径が0以下
    {
        const std::string path = writeTempFile(
            "bad_obstacle_radius",
            "start: [1.0, 1.0]\ngoal: [9.0, 9.0]\nbounds: [0.0, 0.0, 10.0, 10.0]\n"
            "goal_radius: 0.5\nstep_size: 0.5\nmax_iterations: 100\n"
            "obstacles:\n  - center: [5.0, 5.0]\n    radius: 0\n");
        expectThrowsContaining("non-positive obstacle radius", path, "radius");
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
