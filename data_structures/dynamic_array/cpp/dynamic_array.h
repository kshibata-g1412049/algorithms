#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

// 動的配列 — 倍増戦略による自動拡張（テンプレート版）
//
// 任意の型 T を要素として格納できる。
//
// 操作:
//   append(v)      : 末尾に追加  O(1) 均し（容量超過時に倍増）
//   get(i)         : i番目の要素を返す  O(1)
//   set(i, v)      : i番目の要素を設定  O(1)
//   insert(i, v)   : i番目の位置に挿入  O(n)
//   remove(i)      : i番目の要素を削除  O(n)
//   size()         : 要素数  O(1)
//   capacity()     : 確保済み容量  O(1)
//
// インデックス範囲外アクセスは std::out_of_range を投げる。

#include <stdexcept>
#include <vector>

template <typename T>
class DynamicArray {
public:
    DynamicArray() : buf_(4), size_(0) {}

    void append(const T& value) {
        if (size_ == static_cast<int>(buf_.size())) grow();
        buf_[size_++] = value;
    }

    const T& get(int index) const {
        check_index(index);
        return buf_[index];
    }

    void set(int index, const T& value) {
        check_index(index);
        buf_[index] = value;
    }

    void insert(int index, const T& value) {
        if (index < 0 || index > size_)
            throw std::out_of_range("index out of range");
        if (size_ == static_cast<int>(buf_.size())) grow();
        for (int i = size_; i > index; --i) buf_[i] = buf_[i - 1];
        buf_[index] = value;
        ++size_;
    }

    void remove(int index) {
        check_index(index);
        for (int i = index; i < size_ - 1; ++i) buf_[i] = buf_[i + 1];
        --size_;
    }

    int size() const { return size_; }
    int capacity() const { return static_cast<int>(buf_.size()); }

private:
    std::vector<T> buf_;
    int size_;

    void check_index(int index) const {
        if (index < 0 || index >= size_)
            throw std::out_of_range("index out of range");
    }

    void grow() {
        buf_.resize(buf_.size() * 2);
    }
};

#endif // DYNAMIC_ARRAY_H
