#include "segment_tree.h"

SegmentTree::SegmentTree(const std::vector<int>& data)
    : n_(static_cast<int>(data.size())), tree_(4 * n_, 0) {
    if (n_ > 0) build(data, 1, 0, n_ - 1);
}

void SegmentTree::build(const std::vector<int>& data, int node, int lo, int hi) {
    if (lo == hi) { tree_[node] = data[lo]; return; }
    int mid = (lo + hi) / 2;
    build(data, 2 * node, lo, mid);
    build(data, 2 * node + 1, mid + 1, hi);
    tree_[node] = tree_[2 * node] + tree_[2 * node + 1];
}

void SegmentTree::update(int node, int lo, int hi, int i, int value) {
    if (lo == hi) { tree_[node] = value; return; }
    int mid = (lo + hi) / 2;
    if (i <= mid) update(2 * node, lo, mid, i, value);
    else          update(2 * node + 1, mid + 1, hi, i, value);
    tree_[node] = tree_[2 * node] + tree_[2 * node + 1];
}

void SegmentTree::update(int i, int value) {
    if (i < 0 || i >= n_) throw std::out_of_range("index out of range");
    update(1, 0, n_ - 1, i, value);
}

long long SegmentTree::query(int node, int lo, int hi, int l, int r) const {
    if (r < lo || hi < l) return 0;
    if (l <= lo && hi <= r) return tree_[node];
    int mid = (lo + hi) / 2;
    return query(2 * node, lo, mid, l, r) + query(2 * node + 1, mid + 1, hi, l, r);
}

long long SegmentTree::query(int l, int r) const {
    if (l < 0 || r > n_ || l >= r) throw std::out_of_range("invalid range");
    return query(1, 0, n_ - 1, l, r - 1);
}

int SegmentTree::size() const { return n_; }
