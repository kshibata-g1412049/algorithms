#ifndef QUEUE_H
#define QUEUE_H

// キュー（FIFO）— 循環配列バック実装（テンプレート版）
//
// 任意の型 T を要素として格納できる。
//
// 操作:
//   enqueue(v): 末尾に追加  O(1) 均し（容量超過時に倍増）
//   dequeue()  : 先頭を取り出す  O(1) 均し
//   peek()     : 先頭を参照（取り出さない）  O(1)
//   is_empty() : 空かどうか  O(1)
//   size()     : 要素数  O(1)
//
// 空状態での dequeue/peek は std::underflow_error を投げる。

#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
class Queue {
public:
    Queue() : buf_(4), head_(0), count_(0) {}

    void enqueue(const T& value) {
        if (count_ == static_cast<int>(buf_.size())) grow();
        int tail = (head_ + count_) % static_cast<int>(buf_.size());
        buf_[tail] = value;
        ++count_;
    }

    T dequeue() {
        if (count_ == 0) throw std::underflow_error("queue is empty");
        T v = buf_[head_];
        head_ = (head_ + 1) % static_cast<int>(buf_.size());
        --count_;
        return v;
    }

    const T& peek() const {
        if (count_ == 0) throw std::underflow_error("queue is empty");
        return buf_[head_];
    }

    bool is_empty() const { return count_ == 0; }
    int size() const { return count_; }

private:
    std::vector<T> buf_;
    int head_;
    int count_;

    void grow() {
        int newCap = static_cast<int>(buf_.size()) * 2;
        std::vector<T> nb(newCap);
        for (int i = 0; i < count_; ++i)
            nb[i] = buf_[(head_ + i) % static_cast<int>(buf_.size())];
        buf_ = std::move(nb);
        head_ = 0;
    }
};

#endif // QUEUE_H
