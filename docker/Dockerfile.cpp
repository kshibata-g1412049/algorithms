# C++のソートアルゴリズム実装をビルド済みの状態で対話的に試すための環境。
# ビルド時にテスト実行ファイルとCLIサンプル(sort_cli)の両方がビルドされる。
#
# 使い方:
#   docker build -f docker/Dockerfile.cpp -t sorting-cpp .
#   docker run --rm -it sorting-cpp
#
# コンテナ内で実行できること（デフォルトではbashシェルが起動する）:
#   - テストを実行する:   ctest --test-dir build --output-on-failure
#   - CLIで実際に試す:   ./build/sort_cli bubble 5 3 1 4 2
FROM gcc:13

RUN apt-get update \
    && apt-get install -y --no-install-recommends cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY . .

RUN mkdir -p build \
    && cd build \
    && cmake .. \
    && make

CMD ["bash"]
