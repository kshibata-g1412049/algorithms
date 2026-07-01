#include "interpolation_search.h"

int interpolationSearch(const std::vector<int>& arr, int target) {
    int low = 0;
    int high = static_cast<int>(arr.size()) - 1;

    while (low <= high) {
        if (low == high) {
            return (arr[low] == target) ? low : -1;
        }

        // 全要素が同じ値の場合、分母（arr[high]-arr[low]）が0になり
        // ゼロ除算が発生するため、その場合は線形探索的にlowを直接調べる
        if (arr[high] == arr[low]) {
            return (arr[low] == target) ? low : -1;
        }

        // 両端の値とtargetの値から、targetがありそうな位置を線形補間で推定する
        const int pos = low + static_cast<int>(
            (static_cast<long long>(high - low) * (target - arr[low])) /
            (arr[high] - arr[low]));

        // 推定位置が範囲外になる場合（targetが範囲外の値の場合）は探索失敗
        if (pos < low || pos > high) {
            return -1;
        }

        if (arr[pos] == target) {
            return pos;
        }

        if (arr[pos] < target) {
            low = pos + 1; // 推定位置より右側に絞り込む
        } else {
            high = pos - 1; // 推定位置より左側に絞り込む
        }
    }

    return -1;
}
