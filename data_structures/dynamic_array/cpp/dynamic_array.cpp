#include "dynamic_array.h"

DynamicArray::DynamicArray() : buf_(4), size_(0) {}

void DynamicArray::grow() {
    buf_.resize(buf_.size() * 2);
}

void DynamicArray::check_index(int index) const {
    if (index < 0 || index >= size_)
        throw std::out_of_range("index out of range");
}

void DynamicArray::append(int value) {
    if (size_ == static_cast<int>(buf_.size())) grow();
    buf_[size_++] = value;
}

int DynamicArray::get(int index) const {
    check_index(index);
    return buf_[index];
}

void DynamicArray::set(int index, int value) {
    check_index(index);
    buf_[index] = value;
}

void DynamicArray::insert(int index, int value) {
    if (index < 0 || index > size_)
        throw std::out_of_range("index out of range");
    if (size_ == static_cast<int>(buf_.size())) grow();
    for (int i = size_; i > index; --i) buf_[i] = buf_[i - 1];
    buf_[index] = value;
    ++size_;
}

void DynamicArray::remove(int index) {
    check_index(index);
    for (int i = index; i < size_ - 1; ++i) buf_[i] = buf_[i + 1];
    --size_;
}

int DynamicArray::size() const { return size_; }
int DynamicArray::capacity() const { return static_cast<int>(buf_.size()); }
