#include "insertion_sort.h"

void insertionSort(std::vector<int>& arr) {
    const size_t n = arr.size();

    for (size_t i = 1; i < n; ++i) {
        const int key = arr[i]; // 挿入対象の値
        size_t j = i;

        // ソート済み区間 [0, i) の中で key より大きい要素を後ろにずらす
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }

        arr[j] = key; // 正しい位置にkeyを挿入
    }
}
