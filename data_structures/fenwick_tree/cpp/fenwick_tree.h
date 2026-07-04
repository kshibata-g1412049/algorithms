#ifndef FENWICK_TREE_H
#define FENWICK_TREE_H

// フェニック木 (Binary Indexed Tree)（テンプレート版）
//
// 値型 T を汎用化（既定 long long）。int / long long / double などの
// 加算可能な数値型を想定する。T{} がゼロ元として機能すること。
//
// 操作:
//   update(i, delta) : i番目（0始まり）に delta を加算  O(log n)
//   prefix_sum(i)    : [0, i) の前置和  O(log n)
//   range_sum(l, r)  : [l, r) の区間和  O(log n)
//   size()           : 要素数  O(1)
//
// 範囲外アクセスは std::out_of_range を投げる。

#include <stdexcept>
#include <vector>

template <typename T = long long>
class FenwickTree {
public:
    explicit FenwickTree(int n) : n_(n), tree_(n + 1, T{}) {}

    explicit FenwickTree(const std::vector<T>& data)
        : n_(static_cast<int>(data.size())), tree_(data.size() + 1, T{}) {
        for (int i = 0; i < n_; ++i) update(i, data[i]);
    }

    void update(int i, const T& delta) {
        if (i < 0 || i >= n_) throw std::out_of_range("index out of range");
        for (++i; i <= n_; i += i & (-i))
            tree_[i] += delta;
    }

    T prefix_sum(int i) const {
        if (i < 0 || i > n_) throw std::out_of_range("index out of range");
        T s{};
        for (; i > 0; i -= i & (-i)) s += tree_[i];
        return s;
    }

    T range_sum(int l, int r) const {
        if (l < 0 || r > n_ || l > r) throw std::out_of_range("invalid range");
        return prefix_sum(r) - prefix_sum(l);
    }

    int size() const { return n_; }

private:
    int n_;
    std::vector<T> tree_;
};

#endif // FENWICK_TREE_H
