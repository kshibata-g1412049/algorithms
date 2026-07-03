#include "fenwick_tree.h"

FenwickTree::FenwickTree(int n) : n_(n), tree_(n + 1, 0) {}

FenwickTree::FenwickTree(const std::vector<int>& data)
    : n_(static_cast<int>(data.size())), tree_(data.size() + 1, 0) {
    for (int i = 0; i < n_; ++i) update(i, data[i]);
}

void FenwickTree::update(int i, int delta) {
    if (i < 0 || i >= n_) throw std::out_of_range("index out of range");
    for (++i; i <= n_; i += i & (-i))
        tree_[i] += delta;
}

long long FenwickTree::prefix_sum(int i) const {
    if (i < 0 || i > n_) throw std::out_of_range("index out of range");
    long long s = 0;
    for (; i > 0; i -= i & (-i)) s += tree_[i];
    return s;
}

long long FenwickTree::range_sum(int l, int r) const {
    if (l < 0 || r > n_ || l > r) throw std::out_of_range("invalid range");
    return prefix_sum(r) - prefix_sum(l);
}

int FenwickTree::size() const { return n_; }
