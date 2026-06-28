#include "selection_sort.h"
#include <utility>

void selectionSort(std::vector<int>& arr) {
    const size_t n = arr.size();
    if (n < 2) {
        return;
    }

    for (size_t i = 0; i < n - 1; ++i) {
        size_t minIndex = i; // 未確定区間 [i, n) の中で最小値を持つ要素の添字

        // 未確定区間全体を走査して最小値の位置を探す
        for (size_t j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        // 最小値を未確定区間の先頭（i番目）に移動する
        if (minIndex != i) {
            std::swap(arr[i], arr[minIndex]);
        }
    }
}
