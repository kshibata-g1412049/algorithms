#include "counting_sort.h"
#include <algorithm>

void countingSort(std::vector<int>& arr) {
    if (arr.empty()) {
        return;
    }

    // 配列中の最大値を求め、カウント配列のサイズを決める
    const int maxValue = *std::max_element(arr.begin(), arr.end());
    std::vector<int> count(static_cast<size_t>(maxValue) + 1, 0);

    // 各値の出現回数を数え上げる
    for (int value : arr) {
        ++count[value];
    }

    // 累積和を取ることで、各値が出力配列の何番目までの位置に収まるかを求める
    for (size_t i = 1; i < count.size(); ++i) {
        count[i] += count[i - 1];
    }

    // 累積和を使って出力配列を構築する（後ろから走査することで安定性を保つ）
    std::vector<int> output(arr.size());
    for (int i = static_cast<int>(arr.size()) - 1; i >= 0; --i) {
        output[--count[arr[i]]] = arr[i];
    }

    arr = output;
}
