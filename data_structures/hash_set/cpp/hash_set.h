#ifndef HASH_SET_H
#define HASH_SET_H

// ハッシュセット（線形探索法）（テンプレート版）
//
// 任意の型 T を格納できる。ハッシュは Hash（既定 std::hash<T>）で計算し、
// T は == 比較可能であること。
//
// 操作:
//   insert(v)   : O(1)平均
//   contains(v) : O(1)平均
//   remove(v)   : O(1)平均
//   size()      : O(1)
//
// ロードファクタが 0.5 を超えるとリハッシュ（容量2倍）。
// 削除は tombstone マーキングを使用。

#include <functional>
#include <utility>
#include <vector>

template <typename T, typename Hash = std::hash<T>>
class HashSet {
public:
    HashSet() : table_(16) {}

    void insert(const T& value) {
        if (static_cast<double>(size_ + 1) / capacity_ > LOAD_FACTOR) rehash();
        int i = probe(value);
        if (table_[i].state != State::USED) {
            table_[i].value = value;
            table_[i].state = State::USED;
            ++size_;
        }
    }

    bool contains(const T& value) const {
        int i = probe(value);
        return table_[i].state == State::USED && table_[i].value == value;
    }

    bool remove(const T& value) {
        int i = probe(value);
        if (table_[i].state != State::USED || table_[i].value != value) return false;
        table_[i].state = State::DELETED;
        --size_;
        return true;
    }

    int size() const { return size_; }
    bool is_empty() const { return size_ == 0; }

private:
    enum class State { EMPTY, USED, DELETED };
    struct Slot { T value{}; State state = State::EMPTY; };
    std::vector<Slot> table_;
    int size_ = 0;
    int capacity_ = 16;
    Hash hasher_;
    static constexpr double LOAD_FACTOR = 0.5;

    int probe(const T& value) const {
        int i = static_cast<int>(hasher_(value) % static_cast<std::size_t>(capacity_));
        while (table_[i].state == State::USED && !(table_[i].value == value))
            i = (i + 1) % capacity_;
        return i;
    }

    void rehash() {
        std::vector<Slot> old = std::move(table_);
        capacity_ *= 2;
        table_.assign(capacity_, Slot{});
        size_ = 0;
        for (auto& s : old)
            if (s.state == State::USED) insert(s.value);
    }
};

#endif // HASH_SET_H
