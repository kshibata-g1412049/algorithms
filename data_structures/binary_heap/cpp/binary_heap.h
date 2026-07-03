#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

// 二分ヒープ（最小ヒープ）
//
// 操作:
//   insert(v)      : O(log n)
//   find_min()     : O(1)
//   extract_min()  : O(log n)
//   size() / is_empty() : O(1)
//
// 空ヒープでの find_min/extract_min は std::underflow_error を投げる。

#include <stdexcept>
#include <vector>

class BinaryHeap {
public:
    BinaryHeap() = default;

    void insert(int value);
    int find_min() const;
    int extract_min();
    int size() const;
    bool is_empty() const;

private:
    std::vector<int> data_;

    void sift_up(int i);
    void sift_down(int i);
};

#endif // BINARY_HEAP_H
