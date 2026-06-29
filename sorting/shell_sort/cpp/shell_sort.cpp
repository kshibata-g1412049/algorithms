#include "shell_sort.h"

void shellSort(std::vector<int>& arr) {
    const size_t n = arr.size();

    // gapをn/2から開始し、1になるまで半分にしていく
    for (size_t gap = n / 2; gap > 0; gap /= 2) {
        // gap離れた要素同士で挿入ソートを行う
        for (size_t i = gap; i < n; ++i) {
            const int temp = arr[i];
            size_t j = i;

            // gap分離れた手前の要素と比較し、tempより大きければ後ろにずらす
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }

            arr[j] = temp;
        }
    }
}
