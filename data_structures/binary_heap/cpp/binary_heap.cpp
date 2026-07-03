#include "binary_heap.h"
#include <algorithm>

void BinaryHeap::sift_up(int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (data_[p] <= data_[i]) break;
        std::swap(data_[p], data_[i]);
        i = p;
    }
}

void BinaryHeap::sift_down(int i) {
    int n = static_cast<int>(data_.size());
    while (true) {
        int smallest = i;
        int l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && data_[l] < data_[smallest]) smallest = l;
        if (r < n && data_[r] < data_[smallest]) smallest = r;
        if (smallest == i) break;
        std::swap(data_[i], data_[smallest]);
        i = smallest;
    }
}

void BinaryHeap::insert(int value) {
    data_.push_back(value);
    sift_up(static_cast<int>(data_.size()) - 1);
}

int BinaryHeap::find_min() const {
    if (data_.empty()) throw std::underflow_error("heap is empty");
    return data_[0];
}

int BinaryHeap::extract_min() {
    if (data_.empty()) throw std::underflow_error("heap is empty");
    int v = data_[0];
    data_[0] = data_.back();
    data_.pop_back();
    if (!data_.empty()) sift_down(0);
    return v;
}

int BinaryHeap::size() const { return static_cast<int>(data_.size()); }
bool BinaryHeap::is_empty() const { return data_.empty(); }
