// rrt_cli: RRT系5種をコマンドラインから試すためのサンプルプログラム。
//
// 探索領域・障害物・start/goal・各種パラメータは単純なCLI引数では表現しにくい
// ため、YAML設定ファイルのパスを引数で受け取って読み込む方式にしている
// （フォーマットはrrt_cli_config.hを参照）。
//
// リポジトリ全体を集約するroot CMakeLists.txtからのみビルドされる。
//
// 使い方: rrt_cli <algorithm> <config.yaml>
//   例: rrt_cli rrt_star cli/configs/rrt_example.yaml
//
// <algorithm> には rrt, goal_biased_rrt, rrt_star, rrt_connect,
// informed_rrt_star のいずれかを指定する。configでseedを省略した場合は
// std::random_deviceによるランダムなシードを使用する（実行のたびに結果が
// 変わる）。
#include <cstdlib>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include "goal_biased_rrt.h"
#include "informed_rrt_star.h"
#include "rrt.h"
#include "rrt_cli_config.h"
#include "rrt_connect.h"
#include "rrt_star.h"

namespace {

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <algorithm> <config.yaml>\n"
              << "  algorithm: rrt | goal_biased_rrt | rrt_star | rrt_connect"
                 " | informed_rrt_star\n"
              << "  example:   " << programName << " rrt_star cli/configs/rrt_example.yaml\n";
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    const std::string algorithm = argv[1];
    const std::string configPath = argv[2];

    if (algorithm != "rrt" && algorithm != "goal_biased_rrt" && algorithm != "rrt_star" &&
        algorithm != "rrt_connect" && algorithm != "informed_rrt_star") {
        std::cerr << "Error: unknown algorithm '" << algorithm << "'\n";
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    try {
        const RrtCliConfig config = parseRrtCliConfig(configPath);

        unsigned int seed;
        if (config.seed.has_value()) {
            seed = config.seed.value();
        } else {
            std::random_device randomDevice;
            seed = randomDevice();
        }

        std::vector<std::pair<double, double>> path;
        if (algorithm == "rrt") {
            path = rrt(config.obstacles, config.bounds, config.start, config.goal,
                       config.goalRadius, config.stepSize, config.maxIterations, seed);
        } else if (algorithm == "goal_biased_rrt") {
            path = goalBiasedRrt(config.obstacles, config.bounds, config.start, config.goal,
                                  config.goalRadius, config.stepSize, config.maxIterations, seed);
        } else if (algorithm == "rrt_star") {
            path = rrtStar(config.obstacles, config.bounds, config.start, config.goal,
                            config.goalRadius, config.stepSize, config.maxIterations, seed);
        } else if (algorithm == "rrt_connect") {
            path = rrtConnect(config.obstacles, config.bounds, config.start, config.goal,
                               config.goalRadius, config.stepSize, config.maxIterations, seed);
        } else {
            path = informedRrtStar(config.obstacles, config.bounds, config.start, config.goal,
                                    config.goalRadius, config.stepSize, config.maxIterations,
                                    seed);
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
