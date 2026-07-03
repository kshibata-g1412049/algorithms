#ifndef HASH_MAP_H
#define HASH_MAP_H

// ハッシュマップ (int→int、チェイン法)
//
// 操作:
//   insert(key, value)  : O(1)平均
//   get(key)            : O(1)平均  — キーがなければ std::out_of_range
//   remove(key)         : O(1)平均
//   contains(key)       : O(1)平均
//   size()              : O(1)
//
// ロードファクタが 0.75 を超えるとリハッシュ（容量2倍）。

#include <stdexcept>
#include <vector>

class HashMap {
public:
    HashMap();

    void insert(int key, int value);
    int get(int key) const;
    bool remove(int key);
    bool contains(int key) const;
    int size() const;
    bool is_empty() const;

private:
    struct Entry { int key; int value; bool used; };
    std::vector<std::vector<Entry>> buckets_;
    int size_ = 0;
    static constexpr double LOAD_FACTOR = 0.75;

    int bucket_idx(int key) const;
    void rehash();
};

#endif // HASH_MAP_H
