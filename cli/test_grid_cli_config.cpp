// grid_cli_config.cppのテスト。
//
// 正常系（有効なYAML設定の読み込み）と異常系（ファイル未存在・YAML構文
// エラー・必須フィールド欠落・グリッドが矩形でない・start/goalが範囲外
// or障害物上・weightが1.0未満）を網羅する。
#include "grid_cli_config.h"

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
    const std::string path = "/tmp/grid_cli_config_test_" + name + ".yaml";
    std::ofstream out(path);
    out << content;
    out.close();
    return path;
}

void expectThrowsContaining(const std::string& testName, const std::string& path,
                             const std::string& expectedSubstring) {
    try {
        parseGridCliConfig(path);
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
    // 正常系: 有効な設定ファイル（weight省略 -> 既定値1.0）
    {
        const std::string path = writeTempFile(
            "valid", "start: [0, 0]\ngoal: [1, 1]\ngrid:\n  - [0, 0]\n  - [0, 0]\n");
        try {
            const GridCliConfig config = parseGridCliConfig(path);
            if (config.start == std::make_pair(0, 0) && config.goal == std::make_pair(1, 1) &&
                config.grid.size() == 2 && config.weight == 1.0) {
                reportPass("valid config with default weight");
            } else {
                reportFail("valid config with default weight",
                           "parsed values did not match expectations");
            }
        } catch (const std::exception& e) {
            reportFail("valid config with default weight",
                       std::string("unexpected exception: ") + e.what());
        }
    }

    // 正常系: weight指定あり
    {
        const std::string path = writeTempFile(
            "valid_weight",
            "start: [0, 0]\ngoal: [1, 1]\nweight: 1.5\ngrid:\n  - [0, 0]\n  - [0, 0]\n");
        try {
            const GridCliConfig config = parseGridCliConfig(path);
            if (config.weight == 1.5) {
                reportPass("valid config with explicit weight");
            } else {
                reportFail("valid config with explicit weight", "weight did not match");
            }
        } catch (const std::exception& e) {
            reportFail("valid config with explicit weight",
                       std::string("unexpected exception: ") + e.what());
        }
    }

    // 異常系: ファイルが存在しない
    expectThrowsContaining("file not found", "/tmp/grid_cli_config_test_nonexistent.yaml",
                            "not found");

    // 異常系: YAML構文エラー
    {
        const std::string path = writeTempFile("bad_syntax", "start: [0, 0\ngoal: [1, 1]\n");
        expectThrowsContaining("YAML syntax error", path, "parse error");
    }

    // 異常系: 必須フィールド欠落
    {
        const std::string path = writeTempFile("missing_field", "start: [0, 0]\ngoal: [1, 1]\n");
        expectThrowsContaining("missing field 'grid'", path, "grid");
    }

    // 異常系: グリッドが矩形でない
    {
        const std::string path = writeTempFile(
            "non_rect", "start: [0, 0]\ngoal: [1, 1]\ngrid:\n  - [0, 0]\n  - [0]\n");
        expectThrowsContaining("non-rectangular grid", path, "rectangular");
    }

    // 異常系: グリッドのセル値が0/1以外
    {
        const std::string path = writeTempFile(
            "bad_cell", "start: [0, 0]\ngoal: [1, 1]\ngrid:\n  - [0, 2]\n  - [0, 0]\n");
        expectThrowsContaining("invalid cell value", path, "0 or 1");
    }

    // 異常系: startが範囲外
    {
        const std::string path = writeTempFile(
            "start_oob", "start: [5, 5]\ngoal: [0, 0]\ngrid:\n  - [0, 0]\n  - [0, 0]\n");
        expectThrowsContaining("start out of bounds", path, "bounds");
    }

    // 異常系: startが障害物セル上
    {
        const std::string path = writeTempFile(
            "start_on_obs", "start: [1, 1]\ngoal: [0, 0]\ngrid:\n  - [0, 0]\n  - [0, 1]\n");
        expectThrowsContaining("start on obstacle", path, "obstacle");
    }

    // 異常系: weightが1.0未満
    {
        const std::string path = writeTempFile(
            "small_weight",
            "start: [0, 0]\ngoal: [1, 1]\nweight: 0.5\ngrid:\n  - [0, 0]\n  - [0, 0]\n");
        expectThrowsContaining("weight too small", path, "weight");
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
