#include "fibonacci_search.h"
#include <algorithm>

int fibonacciSearch(const std::vector<int>& arr, int target) {
    const int n = static_cast<int>(arr.size());
    if (n == 0) {
        return -1;
    }

    // n以上となる最小のフィボナッチ数列 fibM = fibMMm1 + fibMMm2 を求める
    int fibMMm2 = 0; // F(k-2)
    int fibMMm1 = 1; // F(k-1)
    int fibM = fibMMm2 + fibMMm1; // F(k)

    while (fibM < n) {
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    }

    // offsetは「まだ調べていない先頭側の要素数」を表す
    int offset = -1;

    while (fibM > 1) {
        // F(k-2)分だけ進んだ位置を比較対象にする（配列範囲を超えないようclamp）
        int i = std::min(offset + fibMMm2, n - 1);

        if (arr[i] < target) {
            // targetはiより右側にあるので、[offset, i]の区間を除外し
            // フィボナッチ数列を1つ分小さくして範囲を狭める
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i;
        } else if (arr[i] > target) {
            // targetはiより左側にあるので、フィボナッチ数列を2つ分小さくする
            fibM = fibMMm2;
            fibMMm1 = fibMMm1 - fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        } else {
            return i;
        }
    }

    // 残り1要素（offset+1の位置）を確認する
    if (fibMMm1 == 1 && offset + 1 < n && arr[offset + 1] == target) {
        return offset + 1;
    }

    return -1;
}
