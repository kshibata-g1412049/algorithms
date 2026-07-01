// graph_cli_config.cppのテスト。
//
// 正常系（有効なYAML設定の読み込み）と異常系（ファイル未存在・YAML構文
// エラー・必須フィールド欠落・値の範囲不正）を網羅する。一時ファイルを
// 都度書き出してparseGraphCliConfig()に渡し、期待通りの結果または
// std::runtime_errorが送出されることを確認する。
#include "graph_cli_config.h"

#include <cstdio>
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
    const std::string path = "/tmp/graph_cli_config_test_" + name + ".yaml";
    std::ofstream out(path);
    out << content;
    out.close();
    return path;
}

void expectThrowsContaining(const std::string& testName, const std::string& path,
                             const std::string& expectedSubstring) {
    try {
        parseGraphCliConfig(path);
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
    // 正常系: 有効な設定ファイル
    {
        const std::string path = writeTempFile(
            "valid", "start: 0\nadjacency:\n  - [1, 2]\n  - [0, 3]\n  - [0, 3]\n  - [1, 2]\n");
        try {
            const GraphCliConfig config = parseGraphCliConfig(path);
            if (config.start == 0 && config.adjacency.size() == 4 &&
                config.adjacency[0] == std::vector<int>{1, 2}) {
                reportPass("valid config");
            } else {
                reportFail("valid config", "parsed values did not match expectations");
            }
        } catch (const std::exception& e) {
            reportFail("valid config", std::string("unexpected exception: ") + e.what());
        }
    }

    // 異常系: ファイルが存在しない
    expectThrowsContaining("file not found", "/tmp/graph_cli_config_test_nonexistent.yaml",
                            "not found");

    // 異常系: YAML構文エラー
    {
        const std::string path = writeTempFile("bad_syntax", "start: 0\nadjacency: [1, 2\n");
        expectThrowsContaining("YAML syntax error", path, "parse error");
    }

    // 異常系: 必須フィールド欠落（adjacency）
    {
        const std::string path = writeTempFile("missing_field", "start: 0\n");
        expectThrowsContaining("missing field 'adjacency'", path, "adjacency");
    }

    // 異常系: 必須フィールド欠落（start）
    {
        const std::string path = writeTempFile("missing_start", "adjacency:\n  - []\n");
        expectThrowsContaining("missing field 'start'", path, "start");
    }

    // 異常系: adjacencyが空
    {
        const std::string path = writeTempFile("empty_adjacency", "start: 0\nadjacency: []\n");
        expectThrowsContaining("empty adjacency", path, "empty");
    }

    // 異常系: startが範囲外
    {
        const std::string path =
            writeTempFile("start_oor", "start: 5\nadjacency:\n  - [1]\n  - [0]\n");
        expectThrowsContaining("start out of range", path, "out of range");
    }

    // 異常系: adjacency内のノード番号が範囲外
    {
        const std::string path =
            writeTempFile("neighbor_oor", "start: 0\nadjacency:\n  - [5]\n  - [0]\n");
        expectThrowsContaining("neighbor out of range", path, "out-of-range");
    }

    // 異常系: マッピングでないトップレベル
    {
        const std::string path = writeTempFile("not_mapping", "- 1\n- 2\n");
        expectThrowsContaining("not a mapping", path, "mapping");
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
