#include "bloom_filter.h"

BloomFilter::BloomFilter(std::size_t m, int k) : m_(m), k_(k), bits_(m, false) {}

std::size_t BloomFilter::hash(int value, int seed) const {
    unsigned int h = static_cast<unsigned int>(value) ^ static_cast<unsigned int>(seed * 0x9e3779b9);
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h % m_;
}

void BloomFilter::insert(int value) {
    for (int i = 0; i < k_; ++i)
        bits_[hash(value, i)] = true;
}

bool BloomFilter::contains(int value) const {
    for (int i = 0; i < k_; ++i)
        if (!bits_[hash(value, i)]) return false;
    return true;
}
