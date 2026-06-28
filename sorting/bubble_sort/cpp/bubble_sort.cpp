#include "bubble_sort.h"
#include <utility>

void bubbleSort(std::vector<int>& arr) {
    const size_t n = arr.size();
    if (n < 2) {
        // 要素が0または1個なら既にソート済み
        return;
    }

    for (size_t i = 0; i < n - 1; ++i) {
        bool swapped = false; // このパスで1回も交換が起きなければソート済みと判断できる

        // 未確定区間 [0, n-1-i) を1回スキャンし、隣接要素を比較・交換する
        for (size_t j = 0; j < n - 1 - i; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // 交換が一度も起きなかった = 既にソート済みなので早期終了（最良ケースO(n)を実現）
        if (!swapped) {
            break;
        }
    }
}
