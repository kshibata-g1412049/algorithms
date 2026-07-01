// grid_cli: グリッド経路探索6種をコマンドラインから試すためのサンプルプログラム。
//
// グリッド・start・goal（・weight）は単純なCLI引数では表現しにくいため、
// YAML設定ファイルのパスを引数で受け取って読み込む方式にしている
// （フォーマットはgrid_cli_config.hを参照）。
//
// リポジトリ全体を集約するroot CMakeLists.txtからのみビルドされる。
//
// 使い方: grid_cli <algorithm> <config.yaml>
//   例: grid_cli astar cli/configs/grid_example.yaml
//
// <algorithm> には dijkstra, astar, weighted_astar, bidirectional_astar,
// jps, ida_star のいずれかを指定する。weighted_astarのみconfig内のweightを
// 使用する（省略時は1.0）。
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "astar.h"
#include "bidirectional_astar.h"
#include "dijkstra.h"
#include "grid_cli_config.h"
#include "ida_star.h"
#include "jump_point_search.h"
#include "weighted_astar.h"

namespace {

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <algorithm> <config.yaml>\n"
              << "  algorithm: dijkstra | astar | weighted_astar | bidirectional_astar"
                 " | jps | ida_star\n"
              << "  example:   " << programName << " astar cli/configs/grid_example.yaml\n";
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    const std::string algorithm = argv[1];
    const std::string configPath = argv[2];

    if (algorithm != "dijkstra" && algorithm != "astar" && algorithm != "weighted_astar" &&
        algorithm != "bidirectional_astar" && algorithm != "jps" && algorithm != "ida_star") {
        std::cerr << "Error: unknown algorithm '" << algorithm << "'\n";
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    try {
        const GridCliConfig config = parseGridCliConfig(configPath);

        std::vector<std::pair<int, int>> path;
        if (algorithm == "dijkstra") {
            path = dijkstra(config.grid, config.start, config.goal);
        } else if (algorithm == "astar") {
            path = astar(config.grid, config.start, config.goal);
        } else if (algorithm == "weighted_astar") {
            path = weightedAstar(config.grid, config.start, config.goal, config.weight);
        } else if (algorithm == "bidirectional_astar") {
            path = bidirectionalAstar(config.grid, config.start, config.goal);
        } else if (algorithm == "jps") {
            path = jumpPointSearch(config.grid, config.start, config.goal);
        } else {
            path = idaStar(config.grid, config.start, config.goal);
        }

        if (path.empty()) {
            std::cout << "no path found\n";
        } else {
            for (std::size_t i = 0; i < path.size(); ++i) {
                std::cout << "(" << path[i].first << "," << path[i].second << ")"
                          << (i + 1 < path.size() ? " " : "\n");
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
