#include "union_find.h"
#include <numeric>

UnionFind::UnionFind(int n) : parent_(n), rank_(n, 0), n_(n), components_(n) {
    std::iota(parent_.begin(), parent_.end(), 0);
}

void UnionFind::check(int x) const {
    if (x < 0 || x >= n_) throw std::out_of_range("element out of range");
}

int UnionFind::find(int x) {
    check(x);
    if (parent_[x] != x) parent_[x] = find(parent_[x]);
    return parent_[x];
}

bool UnionFind::unite(int x, int y) {
    int rx = find(x), ry = find(y);
    if (rx == ry) return false;
    if (rank_[rx] < rank_[ry]) std::swap(rx, ry);
    parent_[ry] = rx;
    if (rank_[rx] == rank_[ry]) ++rank_[rx];
    --components_;
    return true;
}

bool UnionFind::connected(int x, int y) { return find(x) == find(y); }
int UnionFind::size() const { return n_; }
int UnionFind::components() const { return components_; }
