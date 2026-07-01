#include "jump_search.h"
#include <algorithm>
#include <cmath>

int jumpSearch(const std::vector<int>& arr, int target) {
    const int n = static_cast<int>(arr.size());
    if (n == 0) {
        return -1;
    }

    // ブロックサイズは√nが理論上最適（ジャンプ回数と線形探索回数のバランスが取れる）
    const int step = static_cast<int>(std::sqrt(static_cast<double>(n)));

    int prev = 0;
    int curr = step;

    // target以上の値を持つブロックが見つかるまでジャンプを続ける
    while (curr < n && arr[curr - 1] < target) {
        prev = curr;
        curr += step;
    }

    // 特定したブロック [prev, min(curr, n)) を先頭から線形探索する
    const int end = std::min(curr, n);
    for (int i = prev; i < end; ++i) {
        if (arr[i] == target) {
            return i;
        }
    }

    return -1;
}
