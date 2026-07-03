#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

// 循環バッファ（リングバッファ）— 固定容量
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

class CircularBuffer {
public:
    explicit CircularBuffer(int capacity);
    void push(int value);
    int pop();
    int peek() const;
    bool is_empty() const;
    bool is_full() const;
    int size() const;
    int capacity() const;

private:
    std::vector<int> buf_;
    int head_;
    int count_;
};

#endif // CIRCULAR_BUFFER_H
