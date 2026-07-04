#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

// 二分ヒープ（最小ヒープ）（テンプレート版）
//
// 任意の型 T を格納できる。順序は Compare（既定 std::less<T>）で決まり、
// Compare で最小の要素が find_min/extract_min の対象になる。
// Compare に std::greater<T> を渡すと最大ヒープとして使える。
//
// 操作:
//   insert(v)      : O(log n)
//   find_min()     : O(1)
//   extract_min()  : O(log n)
//   size() / is_empty() : O(1)
//
// 空ヒープでの find_min/extract_min は std::underflow_error を投げる。

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class BinaryHeap {
public:
    BinaryHeap() = default;

    void insert(const T& value) {
        data_.push_back(value);
        sift_up(static_cast<int>(data_.size()) - 1);
    }

    const T& find_min() const {
        if (data_.empty()) throw std::underflow_error("heap is empty");
        return data_[0];
    }

    T extract_min() {
        if (data_.empty()) throw std::underflow_error("heap is empty");
        T v = data_[0];
        data_[0] = data_.back();
        data_.pop_back();
        if (!data_.empty()) sift_down(0);
        return v;
    }

    int size() const { return static_cast<int>(data_.size()); }
    bool is_empty() const { return data_.empty(); }

private:
    std::vector<T> data_;
    Compare cmp_;

    void sift_up(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (!cmp_(data_[i], data_[p])) break;
            std::swap(data_[p], data_[i]);
            i = p;
        }
    }

    void sift_down(int i) {
        int n = static_cast<int>(data_.size());
        while (true) {
            int smallest = i;
            int l = 2 * i + 1, r = 2 * i + 2;
            if (l < n && cmp_(data_[l], data_[smallest])) smallest = l;
            if (r < n && cmp_(data_[r], data_[smallest])) smallest = r;
            if (smallest == i) break;
            std::swap(data_[i], data_[smallest]);
            i = smallest;
        }
    }
};

#endif // BINARY_HEAP_H
