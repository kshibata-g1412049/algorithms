#ifndef STACK_H
#define STACK_H

// スタック（LIFO）— 動的配列バック実装
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

class Stack {
public:
    void push(int value);
    int pop();
    int peek() const;
    bool is_empty() const;
    int size() const;

private:
    std::vector<int> data_;
};

#endif // STACK_H
