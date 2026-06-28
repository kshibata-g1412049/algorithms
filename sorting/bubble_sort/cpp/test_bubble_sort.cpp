// bubble_sort.cppのテスト。
//
// このファイルは同じフォルダ内のbubble_sort.h/.cppのみに依存しており、
// 外部の共有ヘッダを使わない。そのためcpp/フォルダを単独で取り出しても
// `cmake .. && make && ctest` でビルド・実行できる。
#include "bubble_sort.h"
#include <cstdlib>
#include <iostream>
#include <vector>

// 実際の結果と期待値を比較し、不一致なら詳細を標準エラーに出力するヘルパー。
bool checkEqual(const std::string& testName, const std::vector<int>& actual,
                 const std::vector<int>& expected) {
    if (actual == expected) {
        std::cout << "[PASS] " << testName << std::endl;
        return true;
    }

    std::cerr << "[FAIL] " << testName << ": got {";
    for (size_t i = 0; i < actual.size(); ++i) {
        std::cerr << actual[i] << (i + 1 < actual.size() ? ", " : "");
    }
    std::cerr << "}, expected {";
    for (size_t i = 0; i < expected.size(); ++i) {
        std::cerr << expected[i] << (i + 1 < expected.size() ? ", " : "");
    }
    std::cerr << "}" << std::endl;
    return false;
}

int main() {
    bool allPassed = true;

    // 空配列はそのまま空配列を返す
    {
        std::vector<int> arr = {};
        bubbleSort(arr);
        allPassed &= checkEqual("empty array", arr, {});
    }

    // 要素1つの配列はそのまま
    {
        std::vector<int> arr = {42};
        bubbleSort(arr);
        allPassed &= checkEqual("single element", arr, {42});
    }

    // 既にソート済みの配列
    {
        std::vector<int> arr = {1, 2, 3, 4, 5};
        bubbleSort(arr);
        allPassed &= checkEqual("already sorted", arr, {1, 2, 3, 4, 5});
    }

    // 完全な逆順の配列
    {
        std::vector<int> arr = {5, 4, 3, 2, 1};
        bubbleSort(arr);
        allPassed &= checkEqual("reverse sorted", arr, {1, 2, 3, 4, 5});
    }

    // 重複要素を含む配列
    {
        std::vector<int> arr = {3, 1, 2, 3, 1};
        bubbleSort(arr);
        allPassed &= checkEqual("with duplicates", arr, {1, 1, 2, 3, 3});
    }

    // 負数を含む配列
    {
        std::vector<int> arr = {-3, 5, -1, 0, 2, -8};
        bubbleSort(arr);
        allPassed &= checkEqual("with negative numbers", arr, {-8, -3, -1, 0, 2, 5});
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
