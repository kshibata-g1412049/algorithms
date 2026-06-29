#include "heap_sort.h"
#include <utility>

namespace {

// 添字iを根とする部分木がヒープ条件を満たすように、根を下方向に沈めて調整する。
// heapSizeはヒープとして扱う範囲（[0, heapSize)）を表す。
void siftDown(std::vector<int>& arr, int heapSize, int i) {
    int largest = i;
    const int left = 2 * i + 1;
    const int right = 2 * i + 2;

    if (left < heapSize && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < heapSize && arr[right] > arr[largest]) {
        largest = right;
    }

    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        siftDown(arr, heapSize, largest); // 交換した先でヒープ条件が崩れていないか再帰的に確認
    }
}

} // namespace

void heapSort(std::vector<int>& arr) {
    const int n = static_cast<int>(arr.size());
    if (n < 2) {
        return;
    }

    // 配列全体を最大ヒープに構築する（葉ではない最後の親から逆順に調整）
    for (int i = n / 2 - 1; i >= 0; --i) {
        siftDown(arr, n, i);
    }

    // ヒープの根（最大値）を末尾と交換し、ヒープ範囲を1つ縮めて再調整する
    for (int end = n - 1; end > 0; --end) {
        std::swap(arr[0], arr[end]);
        siftDown(arr, end, 0);
    }
}
