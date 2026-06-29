#include "binary_search.h"

int binarySearch(const std::vector<int>& arr, int target) {
    int low = 0;
    int high = static_cast<int>(arr.size()) - 1;

    while (low <= high) {
        const int mid = low + (high - low) / 2; // オーバーフローを避ける中央値の計算

        if (arr[mid] == target) {
            return mid;
        }

        if (arr[mid] < target) {
            low = mid + 1; // 右半分に絞り込む
        } else {
            high = mid - 1; // 左半分に絞り込む
        }
    }

    return -1;
}
