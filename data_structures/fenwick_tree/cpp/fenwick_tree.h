#ifndef FENWICK_TREE_H
#define FENWICK_TREE_H

// フェニック木 (Binary Indexed Tree)
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

class FenwickTree {
public:
    explicit FenwickTree(int n);
    explicit FenwickTree(const std::vector<int>& data);

    void update(int i, int delta);
    long long prefix_sum(int i) const;
    long long range_sum(int l, int r) const;
    int size() const;

private:
    int n_;
    std::vector<long long> tree_;
};

#endif // FENWICK_TREE_H
