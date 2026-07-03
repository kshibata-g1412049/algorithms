#ifndef QUEUE_H
#define QUEUE_H

// キュー（FIFO）— 循環配列バック実装
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
#include <vector>

class Queue {
public:
    Queue();
    void enqueue(int value);
    int dequeue();
    int peek() const;
    bool is_empty() const;
    int size() const;

private:
    std::vector<int> buf_;
    int head_;
    int count_;

    void grow();
};

#endif // QUEUE_H
