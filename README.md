# algorithms

代表的なソートアルゴリズムを C++ / Python / MATLAB の3言語で実装した学習用リファレンス集です。

## 対象アルゴリズム

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

## ディレクトリ構成

```
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
```

**各言語サブフォルダ（`cpp/`, `python/`, `matlab/`）は単独で取り出してもビルド・実行・テストできます。**
他のフォルダへの依存（共有ヘッダや共通ライブラリ）は一切ありません。

## ビルド・実行・テスト方法

### C++

リポジトリ全体をまとめてビルド・テストする場合:

```sh
mkdir build && cd build
cmake ..
make
ctest --output-on-failure
```

1つのアルゴリズムのcppフォルダだけを取り出して試す場合（例: bubble_sort）:

```sh
cd sorting/bubble_sort/cpp
mkdir build && cd build
cmake ..
make
ctest --output-on-failure
```

### Python

リポジトリ全体のテストをまとめて実行する場合:

```sh
pip install -r requirements.txt
pytest sorting/
```

1つのアルゴリズムのpythonフォルダだけを取り出して試す場合（例: bubble_sort）:

```sh
cd sorting/bubble_sort/python
pip install -r requirements.txt
pytest
```

### MATLAB

各アルゴリズムの `matlab/` フォルダをMATLAB（またはOctave）のパスに追加し、テストスクリプトを実行します（例: bubble_sort）。

```matlab
cd sorting/bubble_sort/matlab
test_bubble_sort
```

全テストが成功すると `All tests passed.` と表示されます。

> 本リポジトリの開発・検証環境にはMATLAB/Octaveがインストールされていないため、MATLAB実装はコードレベルで作成していますが実行検証は行っていません。

## Dockerでの再現手順

C++環境とPython環境それぞれにDockerfileを用意しています。

### C++

```sh
docker build -f docker/Dockerfile.cpp -t sorting-cpp .
docker run --rm sorting-cpp
```

デフォルトでリポジトリ全体をビルドし、`ctest --output-on-failure` を実行します。

### Python

```sh
docker build -f docker/Dockerfile.python -t sorting-python .
docker run --rm sorting-python
```

デフォルトでリポジトリ全体に対して `pytest sorting/ -v` を実行します。

### 単独フォルダだけをDockerで試す場合

ホストの公式イメージにフォルダをマウントして直接ビルドすることもできます（例: bubble_sortのcpp実装）。

```sh
docker run --rm -v "$(pwd)/sorting/bubble_sort/cpp:/work" -w /work gcc:13 \
  bash -c "apt-get update && apt-get install -y cmake && mkdir build && cd build && cmake .. && make && ctest --output-on-failure"
```

## ライセンス

MIT License。詳細は [LICENSE](./LICENSE) を参照してください。
