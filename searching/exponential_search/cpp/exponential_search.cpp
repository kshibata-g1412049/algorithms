#include "exponential_search.h"
#include <algorithm>

namespace {

// [low, high] の範囲に対する標準的な二分探索（内部使用）
int binarySearchRange(const std::vector<int>& arr, int low, int high, int target) {
    while (low <= high) {
        const int mid = low + (high - low) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

} // namespace

int exponentialSearch(const std::vector<int>& arr, int target) {
    const int n = static_cast<int>(arr.size());
    if (n == 0) {
        return -1;
    }

    if (arr[0] == target) {
        return 0;
    }

    // 探索範囲を1, 2, 4, 8, ... と倍々に広げ、target以上の値を持つ
    // 要素が現れる区間（または配列の終端）を特定する
    int bound = 1;
    while (bound < n && arr[bound] < target) {
        bound *= 2;
    }

    // 特定した区間 [bound/2, min(bound, n-1)] に対して二分探索を行う
    const int low = bound / 2;
    const int high = std::min(bound, n - 1);
    return binarySearchRange(arr, low, high, target);
}
