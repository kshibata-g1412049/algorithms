// search_cli: 配列探索7種をコマンドラインから試すためのサンプルプログラム。
//
// リポジトリ全体を集約するroot CMakeLists.txtからのみビルドされる（各アルゴリズムの
// 単独cpp/フォルダのビルドには含まれない）。各アルゴリズムの実装はsearching/<algo>/cpp/
// に既にあるライブラリをそのままリンクして再利用する。
//
// 使い方: search_cli <algorithm> --target N <numbers...>
//   例: search_cli binary --target 5 1 3 5 7 9
//
// <algorithm> には linear, binary, jump, interpolation, exponential, ternary,
// fibonacci のいずれかを指定する。linear以外は入力配列が昇順ソート済みである
// ことを前提とするため、本CLIで事前に検証しエラーにする。
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "binary_search.h"
#include "exponential_search.h"
#include "fibonacci_search.h"
#include "interpolation_search.h"
#include "jump_search.h"
#include "linear_search.h"
#include "ternary_search.h"

namespace {

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <algorithm> --target N <numbers...>\n"
              << "  algorithm: linear | binary | jump | interpolation | exponential"
                 " | ternary | fibonacci\n"
              << "  example:   " << programName << " binary --target 5 1 3 5 7 9\n";
}

bool requiresSortedInput(const std::string& algorithm) {
    return algorithm != "linear";
}

bool isSorted(const std::vector<int>& values) {
    for (std::size_t i = 1; i < values.size(); ++i) {
        if (values[i] < values[i - 1]) {
            return false;
        }
    }
    return true;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 4) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    const std::string algorithm = argv[1];
    const std::string targetFlag = argv[2];
    if (targetFlag != "--target") {
        std::cerr << "Error: expected '--target N' as the second argument\n";
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    char* endPtr = nullptr;
    const long targetLong = std::strtol(argv[3], &endPtr, 10);
    if (endPtr == argv[3] || *endPtr != '\0') {
        std::cerr << "Error: invalid target value '" << argv[3] << "'\n";
        return EXIT_FAILURE;
    }
    const int target = static_cast<int>(targetLong);

    std::vector<int> values;
    for (int i = 4; i < argc; ++i) {
        char* numEndPtr = nullptr;
        const long value = std::strtol(argv[i], &numEndPtr, 10);
        if (numEndPtr == argv[i] || *numEndPtr != '\0') {
            std::cerr << "Error: invalid number '" << argv[i] << "'\n";
            return EXIT_FAILURE;
        }
        values.push_back(static_cast<int>(value));
    }

    if (algorithm != "linear" && algorithm != "binary" && algorithm != "jump" &&
        algorithm != "interpolation" && algorithm != "exponential" &&
        algorithm != "ternary" && algorithm != "fibonacci") {
        std::cerr << "Error: unknown algorithm '" << algorithm << "'\n";
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    if (requiresSortedInput(algorithm) && !isSorted(values)) {
        std::cerr << "Error: " << algorithm
                  << " search requires the input array to be sorted in ascending order\n";
        return EXIT_FAILURE;
    }

    int index = -1;
    if (algorithm == "linear") {
        index = linearSearch(values, target);
    } else if (algorithm == "binary") {
        index = binarySearch(values, target);
    } else if (algorithm == "jump") {
        index = jumpSearch(values, target);
    } else if (algorithm == "interpolation") {
        index = interpolationSearch(values, target);
    } else if (algorithm == "exponential") {
        index = exponentialSearch(values, target);
    } else if (algorithm == "ternary") {
        index = ternarySearch(values, target);
    } else if (algorithm == "fibonacci") {
        index = fibonacciSearch(values, target);
    }

    std::cout << index << "\n";
    return EXIT_SUCCESS;
}
