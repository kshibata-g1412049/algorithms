#ifndef HASH_MAP_H
#define HASH_MAP_H

// ハッシュマップ（チェイン法）（テンプレート版）
//
// 任意のキー型 Key と値型 Value を格納できる。
// ハッシュは Hash（既定 std::hash<Key>）で計算し、Key は == 比較可能であること。
//
// 操作:
//   insert(key, value)  : O(1)平均
//   get(key)            : O(1)平均  — キーがなければ std::out_of_range
//   remove(key)         : O(1)平均
//   contains(key)       : O(1)平均
//   size()              : O(1)
//
// ロードファクタが 0.75 を超えるとリハッシュ（容量2倍）。

#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class HashMap {
public:
    HashMap() : buckets_(16) {}

    void insert(const Key& key, const Value& value) {
        int idx = bucket_idx(key);
        for (auto& e : buckets_[idx]) {
            if (e.key == key) { e.value = value; return; }
        }
        buckets_[idx].push_back({key, value});
        ++size_;
        if (static_cast<double>(size_) / buckets_.size() > LOAD_FACTOR) rehash();
    }

    const Value& get(const Key& key) const {
        int idx = bucket_idx(key);
        for (const auto& e : buckets_[idx])
            if (e.key == key) return e.value;
        throw std::out_of_range("key not found");
    }

    bool remove(const Key& key) {
        int idx = bucket_idx(key);
        auto& chain = buckets_[idx];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->key == key) { chain.erase(it); --size_; return true; }
        }
        return false;
    }

    bool contains(const Key& key) const {
        int idx = bucket_idx(key);
        for (const auto& e : buckets_[idx])
            if (e.key == key) return true;
        return false;
    }

    int size() const { return size_; }
    bool is_empty() const { return size_ == 0; }

private:
    struct Entry { Key key; Value value; };
    std::vector<std::vector<Entry>> buckets_;
    int size_ = 0;
    Hash hasher_;
    static constexpr double LOAD_FACTOR = 0.75;

    int bucket_idx(const Key& key) const {
        return static_cast<int>(hasher_(key) % buckets_.size());
    }

    void rehash() {
        std::vector<std::vector<Entry>> old = std::move(buckets_);
        buckets_.assign(old.size() * 2, {});
        size_ = 0;
        for (auto& chain : old)
            for (auto& e : chain)
                insert(e.key, e.value);
    }
};

#endif // HASH_MAP_H
