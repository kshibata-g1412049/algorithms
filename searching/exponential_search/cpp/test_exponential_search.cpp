// exponential_search.cppのテスト。
//
// このファイルは同じフォルダ内のexponential_search.h/.cppのみに依存しており、
// 外部の共有ヘッダを使わない。そのためcpp/フォルダを単独で取り出しても
// `cmake .. && make && ctest` でビルド・実行できる。
#include "exponential_search.h"
#include <cstdlib>
#include <iostream>
#include <vector>

bool checkEqual(const std::string& testName, int actual, int expected) {
    if (actual == expected) {
        std::cout << "[PASS] " << testName << std::endl;
        return true;
    }

    std::cerr << "[FAIL] " << testName << ": got " << actual
               << ", expected " << expected << std::endl;
    return false;
}

bool checkValidIndex(const std::string& testName, const std::vector<int>& arr,
                      int actual, int target) {
    if (actual >= 0 && static_cast<size_t>(actual) < arr.size() && arr[actual] == target) {
        std::cout << "[PASS] " << testName << std::endl;
        return true;
    }

    std::cerr << "[FAIL] " << testName << ": got index " << actual
               << " which does not point to target " << target << std::endl;
    return false;
}

int main() {
    bool allPassed = true;

    // 空配列は常に-1
    {
        std::vector<int> arr = {};
        allPassed &= checkEqual("empty array", exponentialSearch(arr, 5), -1);
    }

    // 要素1つの配列：一致する場合
    {
        std::vector<int> arr = {42};
        allPassed &= checkEqual("single element, match", exponentialSearch(arr, 42), 0);
    }

    // 要素1つの配列：一致しない場合
    {
        std::vector<int> arr = {42};
        allPassed &= checkEqual("single element, no match", exponentialSearch(arr, 7), -1);
    }

    // targetが先頭にある場合
    {
        std::vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15, 17};
        allPassed &= checkEqual("target at head", exponentialSearch(arr, 1), 0);
    }

    // targetが末尾にある場合
    {
        std::vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15, 17};
        allPassed &= checkEqual("target at tail", exponentialSearch(arr, 17), 8);
    }

    // targetが中央にある場合
    {
        std::vector<int> arr = {1, 3, 5, 7, 9, 11, 13, 15, 17};
        allPassed &= checkEqual("target at middle", exponentialSearch(arr, 9), 4);
    }

    // targetが存在しない場合（最小値より小さい/最大値より大きい/間に挟まれる）
    {
        std::vector<int> arr = {10, 20, 30, 40, 50, 60, 70, 80, 90};
        allPassed &= checkEqual("target smaller than min", exponentialSearch(arr, 5), -1);
        allPassed &= checkEqual("target larger than max", exponentialSearch(arr, 100), -1);
        allPassed &= checkEqual("target between elements", exponentialSearch(arr, 45), -1);
    }

    // 重複要素を含む配列：複数の有効な解があり得るので、返ったインデックスの
    // 値がtargetと一致するかを確認する
    {
        std::vector<int> arr = {2, 4, 4, 4, 6, 8, 10};
        int idx = exponentialSearch(arr, 4);
        allPassed &= checkValidIndex("with duplicates", arr, idx, 4);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
