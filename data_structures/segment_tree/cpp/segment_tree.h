#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

// セグメント木（区間和）（テンプレート版）
//
// 値型 T を汎用化（既定 long long）。int / long long / double などの
// 加算可能な数値型を想定する。T{} がゼロ元として機能すること。
//
// 操作:
//   build(data)          : O(n)
//   update(i, value)     : 0始まりインデックス  O(log n)
//   query(l, r)          : [l, r) の区間和  O(log n)
//   size()               : 要素数  O(1)
//
// 範囲外アクセスは std::out_of_range を投げる。

#include <stdexcept>
#include <vector>

template <typename T = long long>
class SegmentTree {
public:
    explicit SegmentTree(const std::vector<T>& data)
        : n_(static_cast<int>(data.size())), tree_(4 * n_, T{}) {
        if (n_ > 0) build(data, 1, 0, n_ - 1);
    }

    void update(int i, const T& value) {
        if (i < 0 || i >= n_) throw std::out_of_range("index out of range");
        update(1, 0, n_ - 1, i, value);
    }

    T query(int l, int r) const {
        if (l < 0 || r > n_ || l >= r) throw std::out_of_range("invalid range");
        return query(1, 0, n_ - 1, l, r - 1);
    }

    int size() const { return n_; }

private:
    int n_;
    std::vector<T> tree_;

    void build(const std::vector<T>& data, int node, int lo, int hi) {
        if (lo == hi) { tree_[node] = data[lo]; return; }
        int mid = (lo + hi) / 2;
        build(data, 2 * node, lo, mid);
        build(data, 2 * node + 1, mid + 1, hi);
        tree_[node] = tree_[2 * node] + tree_[2 * node + 1];
    }

    void update(int node, int lo, int hi, int i, const T& value) {
        if (lo == hi) { tree_[node] = value; return; }
        int mid = (lo + hi) / 2;
        if (i <= mid) update(2 * node, lo, mid, i, value);
        else          update(2 * node + 1, mid + 1, hi, i, value);
        tree_[node] = tree_[2 * node] + tree_[2 * node + 1];
    }

    T query(int node, int lo, int hi, int l, int r) const {
        if (r < lo || hi < l) return T{};
        if (l <= lo && hi <= r) return tree_[node];
        int mid = (lo + hi) / 2;
        return query(2 * node, lo, mid, l, r) + query(2 * node + 1, mid + 1, hi, l, r);
    }
};

#endif // SEGMENT_TREE_H
