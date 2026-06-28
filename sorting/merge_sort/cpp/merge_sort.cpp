#include "merge_sort.h"

namespace {

// [left, mid] と [mid+1, right] の2つのソート済み区間を1つのソート済み区間にまとめる
void merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> merged;
    merged.reserve(right - left + 1);

    int i = left;
    int j = mid + 1;

    // 両区間の先頭同士を比較し、小さい方を順にmergedへ詰めていく
    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            merged.push_back(arr[i++]);
        } else {
            merged.push_back(arr[j++]);
        }
    }

    // 残った要素をそのまま詰める（どちらか一方は既に詰め終わっている）
    while (i <= mid) {
        merged.push_back(arr[i++]);
    }
    while (j <= right) {
        merged.push_back(arr[j++]);
    }

    // マージ結果を元の配列の対応する範囲に書き戻す
    for (size_t k = 0; k < merged.size(); ++k) {
        arr[left + static_cast<int>(k)] = merged[k];
    }
}

// 区間 [left, right] を再帰的にマージソートする
void mergeSortRange(std::vector<int>& arr, int left, int right) {
    if (left >= right) {
        return; // 要素数が0または1なら既にソート済み
    }

    const int mid = left + (right - left) / 2;
    mergeSortRange(arr, left, mid);
    mergeSortRange(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

} // namespace

void mergeSort(std::vector<int>& arr) {
    if (arr.empty()) {
        return;
    }
    mergeSortRange(arr, 0, static_cast<int>(arr.size()) - 1);
}
