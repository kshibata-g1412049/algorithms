#ifndef DEQUE_H
#define DEQUE_H

// 両端キュー（Deque）— 循環配列バック実装
//
// 操作:
//   push_front(v) / push_back(v)   : 両端への追加  O(1) 均し
//   pop_front()  / pop_back()      : 両端からの取り出し  O(1) 均し
//   peek_front() / peek_back()     : 両端の参照  O(1)
//   is_empty() / size()            : O(1)
//
// 空状態での pop*/peek* は std::underflow_error を投げる。

#include <stdexcept>
#include <vector>

class Deque {
public:
    Deque();
    void push_front(int value);
    void push_back(int value);
    int pop_front();
    int pop_back();
    int peek_front() const;
    int peek_back() const;
    bool is_empty() const;
    int size() const;

private:
    std::vector<int> buf_;
    int head_;
    int count_;
    int cap() const { return static_cast<int>(buf_.size()); }
    void grow();
};

#endif // DEQUE_H
