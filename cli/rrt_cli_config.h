// rrt_cli用のYAML設定ファイル読み込みヘルパー。
//
// rrt_cli（RRT/Goal-biased RRT/RRT*/RRT-Connect/Informed RRT*）が読み込む
// 設定ファイルのフォーマット:
//   start: [0.0, 0.0]
//   goal: [10.0, 10.0]
//   bounds: [0.0, 0.0, 10.0, 10.0]   # xmin, ymin, xmax, ymax
//   goal_radius: 0.5
//   step_size: 0.5
//   max_iterations: 5000
//   seed: 42                          # 省略可（省略時はランダムなシードを使う）
//   obstacles:
//     - center: [5.0, 5.0]
//       radius: 1.0
#ifndef RRT_CLI_CONFIG_H
#define RRT_CLI_CONFIG_H

#include <array>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

struct RrtCliConfig {
    std::vector<std::tuple<double, double, double>> obstacles;
    std::array<double, 4> bounds;
    std::pair<double, double> start;
    std::pair<double, double> goal;
    double goalRadius;
    double stepSize;
    int maxIterations;
    std::optional<unsigned int> seed;
};

// pathからYAML設定ファイルを読み込み、検証して返す。
// ファイルが存在しない、YAML構文エラー、必須フィールド欠落、値の範囲不正の
// 場合はstd::runtime_errorを分かりやすいメッセージ付きで投げる。
RrtCliConfig parseRrtCliConfig(const std::string& path);

#endif  // RRT_CLI_CONFIG_H
