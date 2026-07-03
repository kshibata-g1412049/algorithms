#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

// 動的配列 — 倍増戦略による自動拡張
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

class DynamicArray {
public:
    DynamicArray();
    void append(int value);
    int get(int index) const;
    void set(int index, int value);
    void insert(int index, int value);
    void remove(int index);
    int size() const;
    int capacity() const;

private:
    std::vector<int> buf_;
    int size_;

    void check_index(int index) const;
    void grow();
};

#endif // DYNAMIC_ARRAY_H
