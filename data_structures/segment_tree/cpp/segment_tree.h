#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

// セグメント木（区間和）
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

class SegmentTree {
public:
    explicit SegmentTree(const std::vector<int>& data);

    void update(int i, int value);
    long long query(int l, int r) const;
    int size() const;

private:
    int n_;
    std::vector<long long> tree_;

    void build(const std::vector<int>& data, int node, int lo, int hi);
    void update(int node, int lo, int hi, int i, int value);
    long long query(int node, int lo, int hi, int l, int r) const;
};

#endif // SEGMENT_TREE_H
