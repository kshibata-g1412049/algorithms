#include "hash_map.h"

HashMap::HashMap() : buckets_(16) {}

int HashMap::bucket_idx(int key) const {
    return (static_cast<unsigned int>(key) * 2654435761u) %
           static_cast<unsigned int>(buckets_.size());
}

void HashMap::rehash() {
    std::vector<std::vector<Entry>> old = std::move(buckets_);
    buckets_.assign(old.size() * 2, {});
    size_ = 0;
    for (auto& chain : old)
        for (auto& e : chain)
            insert(e.key, e.value);
}

void HashMap::insert(int key, int value) {
    int idx = bucket_idx(key);
    for (auto& e : buckets_[idx]) {
        if (e.key == key) { e.value = value; return; }
    }
    buckets_[idx].push_back({key, value, true});
    ++size_;
    if (static_cast<double>(size_) / buckets_.size() > LOAD_FACTOR) rehash();
}

int HashMap::get(int key) const {
    int idx = bucket_idx(key);
    for (const auto& e : buckets_[idx])
        if (e.key == key) return e.value;
    throw std::out_of_range("key not found");
}

bool HashMap::remove(int key) {
    int idx = bucket_idx(key);
    auto& chain = buckets_[idx];
    for (auto it = chain.begin(); it != chain.end(); ++it) {
        if (it->key == key) { chain.erase(it); --size_; return true; }
    }
    return false;
}

bool HashMap::contains(int key) const {
    int idx = bucket_idx(key);
    for (const auto& e : buckets_[idx])
        if (e.key == key) return true;
    return false;
}

int HashMap::size() const { return size_; }
bool HashMap::is_empty() const { return size_ == 0; }
