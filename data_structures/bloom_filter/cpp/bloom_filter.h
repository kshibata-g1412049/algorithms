#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

// ブルームフィルタ（テンプレート版）
//
// 任意の型 T を格納できる。基底ハッシュは Hash（既定 std::hash<T>）で計算し、
// シードごとに SplitMix64 系のミキサで独立なハッシュ値を導出する。
//
// 操作:
//   insert(v)   : O(k)
//   contains(v) : O(k) — 偽陽性あり、偽陰性なし
//
// k 個の独立ハッシュ関数で m ビットのビット配列を使用。
// コンストラクタで (m, k) を指定。

#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>

template <typename T, typename Hash = std::hash<T>>
class BloomFilter {
public:
    BloomFilter(std::size_t m, int k) : m_(m), k_(k), bits_(m, false) {}

    void insert(const T& value) {
        for (int i = 0; i < k_; ++i)
            bits_[hash(value, i)] = true;
    }

    bool contains(const T& value) const {
        for (int i = 0; i < k_; ++i)
            if (!bits_[hash(value, i)]) return false;
        return true;
    }

private:
    std::size_t m_;
    int k_;
    std::vector<bool> bits_;
    Hash hasher_;

    std::size_t hash(const T& value, int seed) const {
        std::uint64_t h = static_cast<std::uint64_t>(hasher_(value));
        h ^= static_cast<std::uint64_t>(seed) * 0x9e3779b97f4a7c15ULL;
        h ^= h >> 30; h *= 0xbf58476d1ce4e5b9ULL;
        h ^= h >> 27; h *= 0x94d049bb133111ebULL;
        h ^= h >> 31;
        return static_cast<std::size_t>(h % m_);
    }
};

#endif // BLOOM_FILTER_H
