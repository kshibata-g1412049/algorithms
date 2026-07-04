#ifndef DEQUE_H
#define DEQUE_H

// 両端キュー（Deque）— 循環配列バック実装（テンプレート版）
//
// 任意の型 T を要素として格納できる。
//
// 操作:
//   push_front(v) / push_back(v)   : 両端への追加  O(1) 均し
//   pop_front()  / pop_back()      : 両端からの取り出し  O(1) 均し
//   peek_front() / peek_back()     : 両端の参照  O(1)
//   is_empty() / size()            : O(1)
//
// 空状態での pop*/peek* は std::underflow_error を投げる。

#include <stdexcept>
#include <utility>
#include <vector>

template <typename T>
class Deque {
public:
    Deque() : buf_(4), head_(0), count_(0) {}

    void push_front(const T& value) {
        if (count_ == cap()) grow();
        head_ = (head_ - 1 + cap()) % cap();
        buf_[head_] = value;
        ++count_;
    }

    void push_back(const T& value) {
        if (count_ == cap()) grow();
        buf_[(head_ + count_) % cap()] = value;
        ++count_;
    }

    T pop_front() {
        if (count_ == 0) throw std::underflow_error("deque is empty");
        T v = buf_[head_];
        head_ = (head_ + 1) % cap();
        --count_;
        return v;
    }

    T pop_back() {
        if (count_ == 0) throw std::underflow_error("deque is empty");
        int idx = (head_ + count_ - 1) % cap();
        --count_;
        return buf_[idx];
    }

    const T& peek_front() const {
        if (count_ == 0) throw std::underflow_error("deque is empty");
        return buf_[head_];
    }

    const T& peek_back() const {
        if (count_ == 0) throw std::underflow_error("deque is empty");
        return buf_[(head_ + count_ - 1) % cap()];
    }

    bool is_empty() const { return count_ == 0; }
    int size() const { return count_; }

private:
    std::vector<T> buf_;
    int head_;
    int count_;

    int cap() const { return static_cast<int>(buf_.size()); }

    void grow() {
        int newCap = cap() * 2;
        std::vector<T> nb(newCap);
        for (int i = 0; i < count_; ++i)
            nb[i] = buf_[(head_ + i) % cap()];
        buf_ = std::move(nb);
        head_ = 0;
    }
};

#endif // DEQUE_H
