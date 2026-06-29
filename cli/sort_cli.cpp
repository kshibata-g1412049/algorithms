// sort_cli: 8種のソートアルゴリズムをコマンドラインから試すためのサンプルプログラム。
//
// リポジトリ全体を集約するroot CMakeLists.txtからのみビルドされる（各アルゴリズムの
// 単独cpp/フォルダのビルドには含まれない）。各アルゴリズムの実装はsorting/<algo>/cpp/
// に既にあるライブラリをそのままリンクして再利用する。
//
// 使い方: sort_cli <algorithm> <numbers...>
//   例: sort_cli bubble 5 3 1 4 2
//
// <algorithm> には bubble, selection, insertion, merge, quick, heap, shell,
// counting のいずれかを指定する。countingは非負整数のみ対応。
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "bubble_sort.h"
#include "counting_sort.h"
#include "heap_sort.h"
#include "insertion_sort.h"
#include "merge_sort.h"
#include "quick_sort.h"
#include "selection_sort.h"
#include "shell_sort.h"

namespace {

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <algorithm> <numbers...>\n"
              << "  algorithm: bubble | selection | insertion | merge | quick"
                 " | heap | shell | counting\n"
              << "  example:   " << programName << " bubble 5 3 1 4 2\n";
}

}  // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    const std::string algorithm = argv[1];

    std::vector<int> values;
    for (int i = 2; i < argc; ++i) {
        values.push_back(std::atoi(argv[i]));
    }

    if (algorithm == "counting") {
        for (int v : values) {
            if (v < 0) {
                std::cerr << "Error: counting sort only supports non-negative"
                             " integers (got "
                          << v << ")\n";
                return EXIT_FAILURE;
            }
        }
    }

    if (algorithm == "bubble") {
        bubbleSort(values);
    } else if (algorithm == "selection") {
        selectionSort(values);
    } else if (algorithm == "insertion") {
        insertionSort(values);
    } else if (algorithm == "merge") {
        mergeSort(values);
    } else if (algorithm == "quick") {
        quickSort(values);
    } else if (algorithm == "heap") {
        heapSort(values);
    } else if (algorithm == "shell") {
        shellSort(values);
    } else if (algorithm == "counting") {
        countingSort(values);
    } else {
        std::cerr << "Error: unknown algorithm '" << algorithm << "'\n";
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    for (std::size_t i = 0; i < values.size(); ++i) {
        std::cout << values[i] << (i + 1 < values.size() ? " " : "\n");
    }
    if (values.empty()) {
        std::cout << "\n";
    }

    return EXIT_SUCCESS;
}
