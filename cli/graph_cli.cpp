// graph_cli: BFS/DFSをコマンドラインから試すためのサンプルプログラム。
//
// グラフの隣接リストと開始ノードは単純なCLI引数では表現しにくいため、
// YAML設定ファイルのパスを引数で受け取って読み込む方式にしている
// （フォーマットはgraph_cli_config.hを参照）。
//
// リポジトリ全体を集約するroot CMakeLists.txtからのみビルドされる。
//
// 使い方: graph_cli <algorithm> <config.yaml>
//   例: graph_cli bfs cli/configs/graph_example.yaml
//
// <algorithm> には bfs, dfs のいずれかを指定する。
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "bfs.h"
#include "dfs.h"
#include "graph_cli_config.h"

namespace {

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <algorithm> <config.yaml>\n"
              << "  algorithm: bfs | dfs\n"
              << "  example:   " << programName << " bfs cli/configs/graph_example.yaml\n";
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    const std::string algorithm = argv[1];
    const std::string configPath = argv[2];

    if (algorithm != "bfs" && algorithm != "dfs") {
        std::cerr << "Error: unknown algorithm '" << algorithm << "'\n";
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    try {
        const GraphCliConfig config = parseGraphCliConfig(configPath);

        std::vector<int> order;
        if (algorithm == "bfs") {
            order = bfs(config.adjacency, config.start);
        } else {
            order = dfs(config.adjacency, config.start);
        }

        for (std::size_t i = 0; i < order.size(); ++i) {
            std::cout << order[i] << (i + 1 < order.size() ? " " : "\n");
        }
        if (order.empty()) {
            std::cout << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
