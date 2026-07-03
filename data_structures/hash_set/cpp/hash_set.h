#ifndef HASH_SET_H
#define HASH_SET_H

// ハッシュセット（線形探索法）
//
// 操作:
//   insert(v)   : O(1)平均
//   contains(v) : O(1)平均
//   remove(v)   : O(1)平均
//   size()      : O(1)
//
// ロードファクタが 0.5 を超えるとリハッシュ（容量2倍）。
// 削除は tombstone マーキングを使用。

#include <vector>

class HashSet {
public:
    HashSet();

    void insert(int value);
    bool contains(int value) const;
    bool remove(int value);
    int size() const;
    bool is_empty() const;

private:
    enum class State { EMPTY, USED, DELETED };
    struct Slot { int value; State state; };
    std::vector<Slot> table_;
    int size_ = 0;
    int capacity_ = 16;
    static constexpr double LOAD_FACTOR = 0.5;

    int probe(int value) const;
    void rehash();
};

#endif // HASH_SET_H
