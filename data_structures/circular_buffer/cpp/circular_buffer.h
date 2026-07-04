#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

// 循環バッファ（リングバッファ）— 固定容量（テンプレート版）
//
// 任意の型 T を要素として格納できる。
//
// 操作:
//   push(v)   : 末尾に追加  O(1)  ※満杯なら std::overflow_error
//   pop()     : 先頭を取り出す  O(1)  ※空なら std::underflow_error
//   peek()    : 先頭を参照  O(1)
//   is_full() / is_empty() / size() / capacity() : O(1)
//
// 特徴: 固定サイズの配列と head/tail ポインタでメモリ再確保なし。

#include <stdexcept>
#include <vector>

template <typename T>
class CircularBuffer {
public:
    explicit CircularBuffer(int capacity)
        : buf_(capacity > 0 ? capacity : 0), head_(0), count_(0) {
        if (capacity <= 0) throw std::invalid_argument("capacity must be positive");
    }

    void push(const T& value) {
        if (count_ == static_cast<int>(buf_.size()))
            throw std::overflow_error("circular buffer is full");
        int tail = (head_ + count_) % static_cast<int>(buf_.size());
        buf_[tail] = value;
        ++count_;
    }

    T pop() {
        if (count_ == 0) throw std::underflow_error("circular buffer is empty");
        T v = buf_[head_];
        head_ = (head_ + 1) % static_cast<int>(buf_.size());
        --count_;
        return v;
    }

    const T& peek() const {
        if (count_ == 0) throw std::underflow_error("circular buffer is empty");
        return buf_[head_];
    }

    bool is_empty() const { return count_ == 0; }
    bool is_full() const { return count_ == static_cast<int>(buf_.size()); }
    int size() const { return count_; }
    int capacity() const { return static_cast<int>(buf_.size()); }

private:
    std::vector<T> buf_;
    int head_;
    int count_;
};

#endif // CIRCULAR_BUFFER_H
