// bfs.cppのテスト。
//
// このファイルは同じフォルダ内のbfs.h/.cppのみに依存しており、
// 外部の共有ヘッダを使わない。そのためcpp/フォルダを単独で取り出しても
// `cmake .. && make && ctest` でビルド・実行できる。
#include "bfs.h"
#include <cstdlib>
#include <iostream>
#include <vector>

// 実際の結果と期待値を比較し、不一致なら詳細を標準エラーに出力するヘルパー。
bool checkEqual(const std::string& testName, const std::vector<int>& actual,
                 const std::vector<int>& expected) {
    if (actual == expected) {
        std::cout << "[PASS] " << testName << std::endl;
        return true;
    }

    std::cerr << "[FAIL] " << testName << ": got {";
    for (size_t i = 0; i < actual.size(); ++i) {
        std::cerr << actual[i] << (i + 1 < actual.size() ? ", " : "");
    }
    std::cerr << "}, expected {";
    for (size_t i = 0; i < expected.size(); ++i) {
        std::cerr << expected[i] << (i + 1 < expected.size() ? ", " : "");
    }
    std::cerr << "}" << std::endl;
    return false;
}

int main() {
    bool allPassed = true;

    // 単一ノードのグラフ（隣接リストが空のノード1つ）
    {
        std::vector<std::vector<int>> adjacency = {{}};
        allPassed &= checkEqual("single node", bfs(adjacency, 0), {0});
    }

    // 線形グラフ（0-1-2-3が一直線につながっている）
    {
        std::vector<std::vector<int>> adjacency = {
            {1},      // 0
            {0, 2},   // 1
            {1, 3},   // 2
            {2}       // 3
        };
        allPassed &= checkEqual("linear graph", bfs(adjacency, 0), {0, 1, 2, 3});
    }

    // 分岐のある木構造のグラフ
    //   0 -- 1 -- 3
    //   |    |
    //   2    4
    {
        std::vector<std::vector<int>> adjacency = {
            {1, 2},      // 0
            {0, 3, 4},   // 1
            {0},         // 2
            {1},         // 3
            {1}          // 4
        };
        allPassed &= checkEqual("branching tree", bfs(adjacency, 0), {0, 1, 2, 3, 4});
    }

    // サイクルを含むグラフ（0-1-2-3-0のループ）。無限ループしないことを確認する。
    {
        std::vector<std::vector<int>> adjacency = {
            {1},         // 0
            {2},         // 1
            {0, 3},      // 2
            {2}          // 3
        };
        allPassed &= checkEqual("graph with cycle", bfs(adjacency, 0), {0, 1, 2, 3});
    }

    // 孤立ノードを含む非連結グラフ。startから到達できないノードは結果に含まれない。
    {
        std::vector<std::vector<int>> adjacency = {
            {1},    // 0
            {0},    // 1
            {}      // 2 (孤立ノード、0からは到達不可)
        };
        allPassed &= checkEqual("disconnected graph with isolated node", bfs(adjacency, 0), {0, 1});
    }

    // BFSとDFSで訪問順序が異なることを確認するための、複数の子を持つグラフ
    //   0 -- 1 -- 4
    //   |
    //   2
    //   |
    //   3
    {
        std::vector<std::vector<int>> adjacency = {
            {1, 2, 3},   // 0
            {0, 4},      // 1
            {0},         // 2
            {0},         // 3
            {1}          // 4
        };
        allPassed &= checkEqual("multiple branches", bfs(adjacency, 0), {0, 1, 2, 3, 4});
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
