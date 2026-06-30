# algorithms

[![CI](https://github.com/kshibata-g1412049/algorithms/actions/workflows/ci.yml/badge.svg)](https://github.com/kshibata-g1412049/algorithms/actions/workflows/ci.yml)

代表的なソートアルゴリズム・探索アルゴリズムを C++ / Python / MATLAB の3言語で実装した学習用リファレンス集です。

## 対象アルゴリズム（ソート）

| アルゴリズム | 平均時間複雑度 | 最悪時間複雑度 | 空間複雑度 |
| --- | --- | --- | --- |
| Bubble Sort（バブルソート） | O(n^2) | O(n^2) | O(1) |
| Selection Sort（選択ソート） | O(n^2) | O(n^2) | O(1) |
| Insertion Sort（挿入ソート） | O(n^2) | O(n^2) | O(1) |
| Merge Sort（マージソート） | O(n log n) | O(n log n) | O(n) |
| Quick Sort（クイックソート） | O(n log n) | O(n^2) | O(log n) |
| Heap Sort（ヒープソート） | O(n log n) | O(n log n) | O(1) |
| Shell Sort（シェルソート） | O(n^2)※ | O(n^2)※ | O(1) |
| Counting Sort（計数ソート） | O(n + k) | O(n + k) | O(n + k) |

※ Shell Sortの複雑度はgap列の選び方に依存する。本実装ではShellの原論文のgap列（n/2, n/4, ..., 1）を使用。
Counting SortはO(n + k)のkは配列中の最大値で、非負整数のみを対象とする。

## 対象アルゴリズム（探索）

### 配列・リスト探索（7種）

| アルゴリズム | 平均時間複雑度 | 最悪時間複雑度 | 空間複雑度 |
| --- | --- | --- | --- |
| Linear Search（線形探索） | O(n) | O(n) | O(1) |
| Binary Search（二分探索） | O(log n) | O(log n) | O(1) |
| Jump Search（ジャンプ探索） | O(√n) | O(√n) | O(1) |
| Interpolation Search（補間探索） | O(log log n)※ | O(n) | O(1) |
| Exponential Search（指数探索） | O(log n) | O(log n) | O(1) |
| Ternary Search（三分探索） | O(log₃ n) | O(log₃ n) | O(1) |
| Fibonacci Search（フィボナッチ探索） | O(log n) | O(log n) | O(1) |

※ Interpolation Searchの平均計算量はデータがほぼ一様分布である場合の値。データの分布が偏っている場合は最悪O(n)まで悪化する。
Linear Search以外は**入力配列が昇順にソート済みであること**が前提（事前条件の検証は行わない）。

### 基本グラフ探索（2種）

| アルゴリズム | 時間複雑度 | 空間複雑度 |
| --- | --- | --- |
| BFS（幅優先探索） | O(V + E) | O(V) |
| DFS（深さ優先探索） | O(V + E) | O(V) |

V=ノード数、E=辺数。グラフは0始まりの隣接リストで表現する。

### A*系（6種）

グリッド表現（0=通行可, 1=障害物）・8方向移動（直進コスト1、斜めコスト√2、コーナーカット禁止）・
ヒューリスティックはOctile distanceで統一している。V=セル数、E=辺数。

| アルゴリズム | 時間複雑度 | 空間複雑度 | 備考 |
| --- | --- | --- | --- |
| Dijkstra法 | O(V log V + E) | O(V) | 常に最適解 |
| A* | O(V log V + E) | O(V) | 許容的ヒューリスティックで最適解 |
| Weighted A* | O(V log V + E) | O(V) | weight>1で高速化、最適性は保証されない |
| Bidirectional A* | O(V log V + E) | O(V) | 両方向探索で実用上高速 |
| Jump Point Search (JPS) | O(V log V + E)（最悪） | O(V) | グリッド特有のジャンプ枝刈りで実用上大幅に高速 |
| IDA* | 最悪指数時間 | O(d)（d=探索深さ） | open/closedリストを持たないためA*より大幅に省メモリ |

### RRT系（5種）

2次元連続空間・障害物は円（中心+半径）のリスト・最近傍探索はナイーブな全探索（O(n)）で統一している。
n=ノード数（最大max_iterations）。

| アルゴリズム | 時間複雑度（最悪） | 空間複雑度 | 備考 |
| --- | --- | --- | --- |
| RRT | O(n^2) | O(n) | ゴールバイアスなし |
| Goal-biased RRT | O(n^2) | O(n) | 一定確率でgoal方向へサンプリング |
| RRT* | O(n^2 log n)※ | O(n) | 親選択+リワイヤで漸近的に最適経路へ収束 |
| RRT-Connect | O(n^2) | O(n) | start/goal両方からの双方向木で高速収束 |
| Informed RRT* | O(n^2 log n)※ | O(n) | 解発見後は楕円体領域に絞ってサンプリングし収束を加速 |

※ RRT*/Informed RRT*の近傍探索・リワイヤも本実装ではO(n)のナイーブな線形走査のため、実質O(n^2)程度。
本格的な実装ではいずれもKD-tree等の空間分割構造で最近傍探索を高速化する（本リポジトリでは学習目的のため未実装）。

> **RRT系は乱数を使うため、同じ入力でも実行ごと・言語間で出力される経路は一致しません。**
> 各言語のテストは固定シードによる「その言語内での再現性」と、「start起点・goal半径内終点・障害物非衝突・
> 各ステップがstep_size以下」といった経路の性質を検証するプロパティベーステストになっています。

## ディレクトリ構成

```
cli/                          # 全28アルゴリズムを実際に試せるCLIサンプル
│                              # （sort_cli, search_cli, graph_cli, grid_cli, rrt_cli）
cli/configs/                  # graph_cli/grid_cli/rrt_cli用のサンプルYAML設定ファイル
.github/workflows/ci.yml      # PR時にC++/Python/MATLAB(Octave)のテストを自動実行するCI
sorting/
├── bubble_sort/
│   ├── cpp/       # bubble_sort.h, bubble_sort.cpp, test_bubble_sort.cpp, CMakeLists.txt
│   ├── python/    # bubble_sort.py, test_bubble_sort.py, requirements.txt
│   └── matlab/    # bubble_sort.m, test_bubble_sort.m
├── selection_sort/   (同パターン)
├── insertion_sort/   (同パターン)
├── merge_sort/       (同パターン)
├── quick_sort/       (同パターン)
├── heap_sort/        (同パターン)
├── shell_sort/       (同パターン)
└── counting_sort/    (同パターン。非負整数限定)
searching/
├── linear_search/    (cpp/python/matlabの同パターン。配列・リスト探索)
├── binary_search/        (同パターン)
├── jump_search/          (同パターン)
├── interpolation_search/ (同パターン)
├── exponential_search/   (同パターン)
├── ternary_search/       (同パターン)
├── fibonacci_search/     (同パターン)
├── bfs/                  (同パターン。基本グラフ探索)
├── dfs/                  (同パターン)
├── dijkstra/             (同パターン。A*系)
├── astar/                (同パターン)
├── weighted_astar/       (同パターン)
├── bidirectional_astar/  (同パターン)
├── jump_point_search/    (同パターン)
├── ida_star/             (同パターン)
├── rrt/                  (同パターン。RRT系)
├── goal_biased_rrt/      (同パターン)
├── rrt_star/             (同パターン)
├── rrt_connect/          (同パターン)
└── informed_rrt_star/    (同パターン)
```

**各言語サブフォルダ（`cpp/`, `python/`, `matlab/`）は単独で取り出してもビルド・実行・テストできます。**
他のフォルダへの依存（共有ヘッダや共通ライブラリ）は一切ありません（`searching/*/cpp`はYAMLライブラリにも依存しません。
YAML読み込みは`cli/`層だけに閉じています）。

## ビルド・実行・テスト方法

`sorting/` を `searching/` に置き換えれば同じ手順で探索アルゴリズム側も扱えます（例: `sorting/bubble_sort/cpp` → `searching/binary_search/cpp`）。

### C++

`graph_cli`/`grid_cli`/`rrt_cli`がYAML解析に[yaml-cpp](https://github.com/jbeder/yaml-cpp)を使用するため、ビルド前に導入してください。

```sh
sudo apt-get install -y libyaml-cpp-dev
```

リポジトリ全体をまとめてビルド・テストする場合:

```sh
mkdir build && cd build
cmake ..
make
ctest --output-on-failure
```

1つのアルゴリズムのcppフォルダだけを取り出して試す場合（例: bubble_sort, binary_search）:

```sh
cd sorting/bubble_sort/cpp        # または searching/binary_search/cpp
mkdir build && cd build
cmake ..
make
ctest --output-on-failure
```

> `searching/*/cpp` 配下の各アルゴリズム本体は標準ライブラリのみに依存しており、`yaml-cpp`は不要です（`cli/`層のみの依存）。

### Python

リポジトリ全体のテストをまとめて実行する場合:

```sh
pip install -r requirements.txt
pytest sorting/ searching/ cli/ -v
```

1つのアルゴリズムのpythonフォルダだけを取り出して試す場合（例: bubble_sort, binary_search）:

```sh
cd sorting/bubble_sort/python        # または searching/binary_search/python
pip install -r requirements.txt
pytest
```

### MATLAB

各アルゴリズムの `matlab/` フォルダをMATLAB（またはOctave）のパスに追加し、テストスクリプトを実行します（例: bubble_sort, binary_search）。

```matlab
cd sorting/bubble_sort/matlab        % または searching/binary_search/matlab
test_bubble_sort
```

全テストが成功すると `All tests passed.` と表示されます。

> 本リポジトリの開発・検証環境にはMATLAB/Octaveがインストールされていないため、ローカルでの実行検証は行っていません。ただしCI（GitHub Actions）上ではOctaveをインストールして全28アルゴリズムのテストスクリプトを実際に実行し、動作を検証しています（後述の「CI」を参照）。RRT系のテストは乱数を使うため、固定シードによる再現性とプロパティベースの検証を行っています。

## CLIサンプル（実際に試す）

テストとは別に、コマンドラインから実際にアルゴリズムを試せるサンプルプログラムを `cli/` に用意しています。
各アルゴリズムの実装をそのまま再利用するラッパーで、リポジトリ全体をビルド・セットアップした後に使えます。

### sort_cli（ソート8種）

```sh
# C++（事前にルートで `cmake .. && make` 済みであること）
./build/sort_cli bubble 5 3 1 4 2
# => 1 2 3 4 5

# Python
python cli/sort_cli.py quick 5 3 1 4 2
# => 1 2 3 4 5
```

`<algorithm>` には `bubble`, `selection`, `insertion`, `merge`, `quick`, `heap`, `shell`, `counting` を指定できます。
`counting` は非負整数のみ対応のため、負数を渡すとエラーになります。

### search_cli（配列・リスト探索7種）

`sort_cli`と同じく、単純な位置引数のみで完結します。

```sh
# C++
./build/search_cli binary --target 5 1 3 5 7 9
# => 2

# Python
python cli/search_cli.py binary --target 5 1 3 5 7 9
# => 2
```

`<algorithm>` には `linear`, `binary`, `jump`, `interpolation`, `exponential`, `ternary`, `fibonacci` を指定できます。
`linear`以外は入力配列が昇順ソート済みであることを要求し、違反するとエラーになります（見つからない場合は`-1`を出力）。

### graph_cli / grid_cli / rrt_cli（グラフ・グリッド・連続空間探索）

グラフの隣接リスト・グリッド・障害物地図といった入力は単純なCLI引数では表現しにくいため、
**YAML設定ファイルのパスを引数で受け取って読み込む方式**にしています。サンプル設定ファイルは
`cli/configs/` 以下にあります。

```sh
# graph_cli: BFS/DFS
./build/graph_cli bfs cli/configs/graph_example.yaml
python cli/graph_cli.py bfs cli/configs/graph_example.yaml
# => 0 1 2 3

# grid_cli: Dijkstra/A*/Weighted A*/Bidirectional A*/JPS/IDA*
./build/grid_cli astar cli/configs/grid_example.yaml
python cli/grid_cli.py astar cli/configs/grid_example.yaml
# => (0,0) (1,0) (2,0) ... (4,4)

# rrt_cli: RRT/Goal-biased RRT/RRT*/RRT-Connect/Informed RRT*
./build/rrt_cli rrt_star cli/configs/rrt_example.yaml
python cli/rrt_cli.py rrt_star cli/configs/rrt_example.yaml
# => (1.0,1.0) ... (8.x,8.x)（乱数を使うため実行ごとに経路は変わる）
```

`graph_cli`の`<algorithm>`には`bfs`, `dfs`、`grid_cli`には`dijkstra`, `astar`, `weighted_astar`,
`bidirectional_astar`, `jps`, `ida_star`、`rrt_cli`には`rrt`, `goal_biased_rrt`, `rrt_star`,
`rrt_connect`, `informed_rrt_star`を指定できます。

設定ファイルのフォーマット例（`cli/configs/grid_example.yaml`）:

```yaml
start: [0, 0]
goal: [4, 4]
weight: 1.5            # weighted_astar以外では無視される
grid:
  - [0, 0, 0, 0, 0]
  - [0, 1, 1, 1, 0]
  - [0, 0, 0, 0, 0]
  - [0, 1, 1, 1, 0]
  - [0, 0, 0, 0, 0]
```

**例外処理**: 4種のCLI全てで、以下のケースを分かりやすいエラーメッセージ＋非ゼロ終了コードで検出します。

- 引数不足・不明なアルゴリズム名
- 設定ファイルが存在しない／読み込めない
- YAML構文エラー
- 必須フィールドの欠落・型不正
- 値の範囲不正（例: `graph_cli`はノード番号が範囲外、`grid_cli`はグリッドが矩形でない／start・goalが
  範囲外or障害物セル上／weightが1.0未満、`rrt_cli`はboundsが不正／start・goalがbounds外or障害物内／
  goal_radius・step_size・max_iterations・障害物半径が0以下）

```sh
$ ./build/graph_cli bfs cli/configs/does_not_exist.yaml
Error: config file not found or not readable: cli/configs/does_not_exist.yaml
$ echo $?
1
```

## CI

GitHub Actions（`.github/workflows/ci.yml`）で、`main` ブランチへのPull Request作成・更新時と `main` へのpush時に、以下の3ジョブを自動実行しています。

- **cpp**: `apt-get install -y libyaml-cpp-dev` の後、ルートの集約ビルドで `cmake .. && make` した後 `ctest --output-on-failure`（CLI設定読み込みの単体テスト・CLIバイナリのスモークテストを含む）
- **python**: `pip install -r requirements.txt` の後 `pytest sorting/ searching/ cli/ -v`（CLIの例外処理テストを含む）
- **matlab**: `apt-get install octave` でOctaveを導入し、`sorting/`・`searching/`双方の全アルゴリズムの `test_*.m` を実行

## Dockerでの再現手順

C++環境とPython環境それぞれにDockerfileを用意しています。コンテナはデフォルトで
bashシェルを起動するので、テスト実行とCLIサンプルの両方を対話的に試せます。

### C++

```sh
docker build -f docker/Dockerfile.cpp -t sorting-cpp .
docker run --rm -it sorting-cpp
```

`Dockerfile.cpp`はビルド時に`libyaml-cpp-dev`を自動導入します。コンテナ内（リポジトリ全体は既にビルド済み）:

```sh
ctest --test-dir build --output-on-failure   # テストを実行
./build/sort_cli bubble 5 3 1 4 2            # CLIで実際にソートを試す
./build/grid_cli astar cli/configs/grid_example.yaml   # CLIで実際に探索を試す
```

### Python

```sh
docker build -f docker/Dockerfile.python -t sorting-python .
docker run --rm -it sorting-python
```

コンテナ内:

```sh
pytest sorting/ searching/ cli/ -v       # テストを実行
python cli/sort_cli.py quick 5 3 1 4 2   # CLIで実際にソートを試す
python cli/grid_cli.py astar cli/configs/grid_example.yaml   # CLIで実際に探索を試す
```

### 単独フォルダだけをDockerで試す場合

ホストの公式イメージにフォルダをマウントして直接ビルドすることもできます（例: bubble_sortのcpp実装）。

```sh
docker run --rm -v "$(pwd)/sorting/bubble_sort/cpp:/work" -w /work gcc:13 \
  bash -c "apt-get update && apt-get install -y cmake && mkdir build && cd build && cmake .. && make && ctest --output-on-failure"
```

## ライセンス

MIT License。詳細は [LICENSE](./LICENSE) を参照してください。
