// graph_cli用のYAML設定ファイル読み込みヘルパー。
//
// graph_cli（BFS/DFS）が読み込む設定ファイルのフォーマット:
//   start: 0
//   adjacency:
//     - [1, 2]
//     - [0, 3]
//     - [0, 3]
//     - [1, 2]
//
// start: 探索開始ノード（0始まり）。
// adjacency: 各ノードの隣接リスト（0始まりのノード番号の配列の配列）。
//            添字iの要素がノードiの隣接ノード一覧を表す。
#ifndef GRAPH_CLI_CONFIG_H
#define GRAPH_CLI_CONFIG_H

#include <string>
#include <vector>

struct GraphCliConfig {
    int start;
    std::vector<std::vector<int>> adjacency;
};

// pathからYAML設定ファイルを読み込み、検証して返す。
// ファイルが存在しない、YAML構文エラー、必須フィールド欠落、値の範囲不正の
// 場合はstd::runtime_errorを分かりやすいメッセージ付きで投げる。
GraphCliConfig parseGraphCliConfig(const std::string& path);

#endif  // GRAPH_CLI_CONFIG_H
