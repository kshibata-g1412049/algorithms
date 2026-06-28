#include "quick_sort.h"
#include <utility>

namespace {

// Lomutoパーティション方式: 区間 [low, high] の末尾要素をピボットとし、
// ピボットより小さい要素を左側に集める。最終的なピボットの位置を返す。
int partition(std::vector<int>& arr, int low, int high) {
    const int pivot = arr[high];
    int i = low - 1; // ピボットより小さい要素の末尾位置

    for (int j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]); // ピボットを正しい位置（区間の境界）に移動
    return i + 1;
}

// 区間 [low, high] を再帰的にクイックソートする
void quickSortRange(std::vector<int>& arr, int low, int high) {
    if (low >= high) {
        return; // 要素数が0または1なら既にソート済み
    }

    const int pivotIndex = partition(arr, low, high);
    quickSortRange(arr, low, pivotIndex - 1);
    quickSortRange(arr, pivotIndex + 1, high);
}

} // namespace

void quickSort(std::vector<int>& arr) {
    if (arr.empty()) {
        return;
    }
    quickSortRange(arr, 0, static_cast<int>(arr.size()) - 1);
}
