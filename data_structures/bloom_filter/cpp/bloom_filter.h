#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

// ブルームフィルタ
//
// 操作:
//   insert(v)   : O(k)
//   contains(v) : O(k) — 偽陽性あり、偽陰性なし
//
// k 個の独立ハッシュ関数で m ビットのビット配列を使用。
// コンストラクタで (m, k) を指定。

#include <cstddef>
#include <vector>

class BloomFilter {
public:
    BloomFilter(std::size_t m, int k);

    void insert(int value);
    bool contains(int value) const;

private:
    std::size_t m_;
    int k_;
    std::vector<bool> bits_;

    std::size_t hash(int value, int seed) const;
};

#endif // BLOOM_FILTER_H
