# algorithms

[![CI](https://github.com/kshibata-g1412049/algorithms/actions/workflows/ci.yml/badge.svg)](https://github.com/kshibata-g1412049/algorithms/actions/workflows/ci.yml)

代表的なソートアルゴリズム・探索アルゴリズム・データ構造を C++ / Python / MATLAB の3言語で実装した学習用リファレンス集です。

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

## 対象データ構造（20種）

各データ構造は C++（テンプレートクラス）・Python（`Generic` 型ヒント付きクラス）・MATLAB
（ディスパッチ関数＋struct）で実装しています。C++/Python は**任意の型を要素にできる**汎用実装です
（例外は後述の「int 固定の3種」）。

### 線形（7種）

| データ構造 | 主要操作 | 計算量 |
| --- | --- | --- |
| Stack（スタック） | push / pop / peek | すべて O(1) |
| Queue（キュー） | enqueue / dequeue / peek | すべて O(1) 均し |
| Deque（両端キュー） | push_front / push_back / pop_front / pop_back | すべて O(1) 均し |
| Singly Linked List（単方向連結リスト） | push_front / pop_front / contains / remove | 先頭 O(1)、探索 O(n) |
| Doubly Linked List（双方向連結リスト） | push/pop 両端 / contains / remove | 両端 O(1)、探索 O(n) |
| Circular Buffer（循環バッファ） | push / pop / peek（固定容量） | すべて O(1) |
| Dynamic Array（動的配列） | append / get / set / insert / remove | append O(1) 均し、insert/remove O(n) |

### 木構造（6種）

| データ構造 | 主要操作 | 計算量 |
| --- | --- | --- |
| Binary Search Tree（二分探索木） | insert / search / remove / min / max / 3走査 | 平均 O(log n)、最悪 O(n) |
| AVL Tree（AVL木） | insert / search / remove（回転で自己平衡） | O(log n) 保証 |
| Binary Heap（二分ヒープ・最小ヒープ） | insert / find_min / extract_min | insert/extract O(log n)、find O(1) |
| Trie（トライ木） | insert / search / starts_with / remove | O(L)、L=文字列長 |
| Segment Tree（セグメント木・区間和） | update / query | いずれも O(log n)、構築 O(n) |
| Fenwick Tree（フェニック木 / BIT） | update / prefix_sum / range_sum | すべて O(log n) |

### ハッシュ（2種）

| データ構造 | 実装方式 | 計算量 |
| --- | --- | --- |
| Hash Map（ハッシュマップ） | チェイン法、ロードファクタ0.75でリハッシュ | 平均 O(1) |
| Hash Set（ハッシュセット） | 線形探索法（tombstone削除）、ロードファクタ0.5 | 平均 O(1) |

### グラフ表現（3種）

| データ構造 | 主要操作 | 計算量 |
| --- | --- | --- |
| Adjacency List Graph（隣接リスト） | add_vertex / add_edge / get_neighbors | add O(1)、has_edge O(deg) |
| Adjacency Matrix Graph（隣接行列） | add_edge / remove_edge / has_edge | has_edge O(1)、get_neighbors O(V) |
| Union-Find（素集合） | find（経路圧縮）/ unite（ランク結合）/ connected | O(α(n)) ≒ O(1) 均し |

### 確率的（2種）

| データ構造 | 主要操作 | 特徴 |
| --- | --- | --- |
| Bloom Filter（ブルームフィルタ） | insert / contains | 偽陽性あり・偽陰性なし、O(k) |
| Skip List（スキップリスト） | insert / search / remove | 平均 O(log n)。**MATLAB実装なし**（可変長前方ポインタ配列がMATLABの配列モデルと相性が悪いため） |

### テンプレート／ジェネリクスの使い方

C++ 実装はテンプレートクラスです。テンプレート引数で要素型・比較器・ハッシュ関数を差し替えられます。

```cpp
Stack<std::string> s;                        // 任意の要素型（int以外もOK）
s.push("hello");

BinaryHeap<int, std::greater<int>> maxheap;  // Compare を差し替えると最大ヒープになる
maxheap.insert(10);
maxheap.extract_min();                       // => 10（Compare上の「最小」= 数値の最大）

HashMap<std::string, std::string> m;         // 任意のキー型・値型（Hash は既定で std::hash）
m.insert("apple", "red");

AdjacencyListGraph<std::string> g;           // 頂点を文字列ラベルにできる
g.add_edge("tokyo", "osaka");

std::vector<double> data = {0.5, 1.5, 2.0};
SegmentTree<double> st(data);                // 実数の区間和（既定は long long）
FenwickTree<double> ft(3);                   // 同上
```

- 順序付き構造（BST / AVL / Heap / Skip List）は `template<typename T, typename Compare = std::less<T>>`。
  等価判定も比較器から導出（`!cmp(a,b) && !cmp(b,a)`）するため、`<` が定義された型ならそのまま使えます。
- ハッシュ系（Hash Map / Hash Set / Bloom Filter / 隣接リスト）は `Hash = std::hash<...>` を既定とし、
  独自ハッシュ関数オブジェクトを渡せます。
- テンプレート実装はすべてヘッダ（`.h`）内にあり、`.cpp` はビルドターゲット維持用のスタブです。

Python 実装は `typing.Generic` による型ヒント付きで、任意の型を実行時にそのまま格納できます。

```python
s: Stack[str] = Stack()
s.push("hello")

m: HashMap[str, str] = HashMap()
m.insert("apple", "red")

g: AdjacencyListGraph[str] = AdjacencyListGraph()
g.add_edge("tokyo", "osaka")
```

**int 固定の3種**: `trie`（a〜z 26文字アルファベット特化が設計意図）、`adjacency_matrix_graph`
（頂点IDが行列インデックスそのもの）、`union_find`（parent/rank 配列が整数インデックス前提）は
汎用化の対象外です。

**MATLAB**: 動的型付けのため、数値型（double / int32 等）はコード変更なしでそのまま扱えます。
文字列を順序付き構造に入れる場合は `<` 演算子の代わりに `strcmp` ベースの比較が必要になるため
サポート外です。

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
data_structures/
├── stack/
│   ├── cpp/       # stack.h（テンプレート実装）, stack.cpp（スタブ）, test_stack.cpp, CMakeLists.txt
│   ├── python/    # stack.py, test_stack.py
│   └── matlab/    # stack.m, test_stack.m
├── queue/                    (同パターン。線形7種)
├── deque/                    (同パターン)
├── singly_linked_list/       (同パターン)
├── doubly_linked_list/       (同パターン)
├── circular_buffer/          (同パターン)
├── dynamic_array/            (同パターン)
├── binary_search_tree/       (同パターン。木構造6種)
├── avl_tree/                 (同パターン)
├── binary_heap/              (同パターン)
├── trie/                     (同パターン)
├── segment_tree/             (同パターン)
├── fenwick_tree/             (同パターン)
├── hash_map/                 (同パターン。ハッシュ2種)
├── hash_set/                 (同パターン)
├── adjacency_list_graph/     (同パターン。グラフ3種)
├── adjacency_matrix_graph/   (同パターン)
├── union_find/               (同パターン)
├── bloom_filter/             (同パターン。確率的2種)
└── skip_list/                (cpp/pythonのみ。MATLAB実装なし)
```

**各言語サブフォルダ（`cpp/`, `python/`, `matlab/`）は単独で取り出してもビルド・実行・テストできます。**
他のフォルダへの依存（共有ヘッダや共通ライブラリ）は一切ありません（`searching/*/cpp`はYAMLライブラリにも依存しません。
YAML読み込みは`cli/`層だけに閉じています）。

## ビルド・実行・テスト方法

`sorting/` を `searching/` や `data_structures/` に置き換えれば同じ手順で探索アルゴリズム・データ構造側も扱えます
（例: `sorting/bubble_sort/cpp` → `searching/binary_search/cpp` → `data_structures/stack/cpp`）。

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
pytest sorting/ searching/ data_structures/ cli/ -v
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

> CI（GitHub Actions）上ではOctaveをインストールして全28アルゴリズム＋データ構造19種（skip_listを除く）のテストスクリプトを実際に実行し、動作を検証しています（後述の「CI」を参照）。RRT系のテストは乱数を使うため、固定シードによる再現性とプロパティベースの検証を行っています。

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
- **python**: `pip install -r requirements.txt` の後 `pytest sorting/ searching/ data_structures/ cli/ -v`（CLIの例外処理テストを含む）
- **matlab**: `apt-get install octave` でOctaveを導入し、`sorting/`・`searching/`・`data_structures/` の全 `test_*.m` を実行

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
