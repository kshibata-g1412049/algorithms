// grid_cli用のYAML設定ファイル読み込みヘルパー。
//
// grid_cli（Dijkstra/A*/Weighted A*/Bidirectional A*/Jump Point Search/IDA*）が
// 読み込む設定ファイルのフォーマット:
//   start: [0, 0]
//   goal: [4, 4]
//   weight: 1.5            # weighted_astar以外では無視される（省略可、既定1.0）
//   grid:
//     - [0, 0, 0, 0, 0]
//     - [0, 1, 1, 1, 0]
//     - [0, 0, 0, 0, 0]
//     - [0, 1, 1, 1, 0]
//     - [0, 0, 0, 0, 0]
//
// grid: 2次元配列。0=通行可能, 1=障害物。全行が同じ長さの矩形であること。
// start, goal: [row, col]。グリッド範囲内かつ通行可能マス（0）であること。
// weight: ヒューリスティックの重み。指定する場合は1.0以上であること。
#ifndef GRID_CLI_CONFIG_H
#define GRID_CLI_CONFIG_H

#include <string>
#include <utility>
#include <vector>

struct GridCliConfig {
    std::vector<std::vector<int>> grid;
    std::pair<int, int> start;
    std::pair<int, int> goal;
    double weight = 1.0;
};

// pathからYAML設定ファイルを読み込み、検証して返す。
// ファイルが存在しない、YAML構文エラー、必須フィールド欠落、値の範囲不正の
// 場合はstd::runtime_errorを分かりやすいメッセージ付きで投げる。
GridCliConfig parseGridCliConfig(const std::string& path);

#endif  // GRID_CLI_CONFIG_H
