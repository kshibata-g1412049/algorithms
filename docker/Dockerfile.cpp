# C++のソートアルゴリズム実装をビルドし、CTestでテストを実行するための環境。
#
# 使い方:
#   docker build -f docker/Dockerfile.cpp -t sorting-cpp .
#   docker run --rm sorting-cpp
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

WORKDIR /workspace/build
CMD ["ctest", "--output-on-failure"]
