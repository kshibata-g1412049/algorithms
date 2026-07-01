#include "ternary_search.h"

int ternarySearch(const std::vector<int>& arr, int target) {
    int low = 0;
    int high = static_cast<int>(arr.size()) - 1;

    while (low <= high) {
        // 探索範囲を3等分する2つの分割点
        const int mid1 = low + (high - low) / 3;
        const int mid2 = high - (high - low) / 3;

        if (arr[mid1] == target) {
            return mid1;
        }
        if (arr[mid2] == target) {
            return mid2;
        }

        if (target < arr[mid1]) {
            high = mid1 - 1; // 第1区間に絞り込む
        } else if (target > arr[mid2]) {
            low = mid2 + 1; // 第3区間に絞り込む
        } else {
            low = mid1 + 1; // 第2区間（中央）に絞り込む
            high = mid2 - 1;
        }
    }

    return -1;
}
