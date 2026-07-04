#ifndef STACK_H
#define STACK_H

// スタック（LIFO）— 動的配列バック実装（テンプレート版）
//
// 任意の型 T を要素として格納できる。
//
// 操作:
//   push(v)  : 要素を積む  O(1) 均し
//   pop()    : 先頭を取り出す  O(1)
//   peek()   : 先頭を参照（取り出さない）  O(1)
//   is_empty(): 空かどうか  O(1)
//   size()   : 要素数  O(1)
//
// 空状態での pop/peek は std::underflow_error を投げる。

#include <stdexcept>
#include <vector>

template <typename T>
class Stack {
public:
    void push(const T& value) {
        data_.push_back(value);
    }

    T pop() {
        if (data_.empty()) throw std::underflow_error("stack is empty");
        T v = data_.back();
        data_.pop_back();
        return v;
    }

    const T& peek() const {
        if (data_.empty()) throw std::underflow_error("stack is empty");
        return data_.back();
    }

    bool is_empty() const { return data_.empty(); }

    int size() const { return static_cast<int>(data_.size()); }

private:
    std::vector<T> data_;
};

#endif // STACK_H
