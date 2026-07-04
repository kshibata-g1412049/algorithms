#ifndef SKIP_LIST_H
#define SKIP_LIST_H

// スキップリスト（確率的データ構造）（テンプレート版）
//
// 任意の型 T を格納できる。順序は Compare（既定 std::less<T>）で決まる。
// 先頭センチネルは std::optional の nullopt で表し、常にどの値よりも小さいとみなす。
//
// 操作:
//   insert(v)   : 平均 O(log n)
//   search(v)   : 平均 O(log n)
//   remove(v)   : 平均 O(log n)
//   size()      : O(1)
//
// 最大レベル 16、各レベル昇格確率 0.5。
// 再現性のためコンストラクタにシードを受け取る。

#include <cstdint>
#include <functional>
#include <optional>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class SkipList {
public:
    explicit SkipList(uint32_t seed = 42)
        : head_(new Node(MAX_LEVEL)), rng_(seed) {}

    ~SkipList() {
        Node* cur = head_;
        while (cur) { Node* n = cur->forward[0]; delete cur; cur = n; }
    }

    SkipList(const SkipList&) = delete;
    SkipList& operator=(const SkipList&) = delete;

    void insert(const T& value) {
        std::vector<Node*> update(MAX_LEVEL + 1);
        Node* cur = head_;
        for (int i = level_; i >= 0; --i) {
            while (cur->forward[i] && cmp_(*cur->forward[i]->val, value))
                cur = cur->forward[i];
            update[i] = cur;
        }
        cur = cur->forward[0];
        if (cur && equal(*cur->val, value)) return;
        int lvl = random_level();
        if (lvl > level_) {
            for (int i = level_ + 1; i <= lvl; ++i) update[i] = head_;
            level_ = lvl;
        }
        Node* n = new Node(value, lvl);
        for (int i = 0; i <= lvl; ++i) {
            n->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = n;
        }
        ++size_;
    }

    bool search(const T& value) const {
        Node* cur = head_;
        for (int i = level_; i >= 0; --i)
            while (cur->forward[i] && cmp_(*cur->forward[i]->val, value))
                cur = cur->forward[i];
        cur = cur->forward[0];
        return cur && equal(*cur->val, value);
    }

    bool remove(const T& value) {
        std::vector<Node*> update(MAX_LEVEL + 1);
        Node* cur = head_;
        for (int i = level_; i >= 0; --i) {
            while (cur->forward[i] && cmp_(*cur->forward[i]->val, value))
                cur = cur->forward[i];
            update[i] = cur;
        }
        cur = cur->forward[0];
        if (!cur || !equal(*cur->val, value)) return false;
        for (int i = 0; i <= level_; ++i) {
            if (update[i]->forward[i] != cur) break;
            update[i]->forward[i] = cur->forward[i];
        }
        delete cur;
        while (level_ > 0 && !head_->forward[level_]) --level_;
        --size_;
        return true;
    }

    int size() const { return size_; }
    bool is_empty() const { return size_ == 0; }

private:
    static constexpr int MAX_LEVEL = 16;

    struct Node {
        std::optional<T> val;  // nullopt = 先頭センチネル
        std::vector<Node*> forward;
        explicit Node(int level) : forward(level + 1, nullptr) {}
        Node(const T& v, int level) : val(v), forward(level + 1, nullptr) {}
    };

    Node* head_;
    int level_ = 0;
    int size_ = 0;
    uint32_t rng_;
    Compare cmp_;

    bool equal(const T& a, const T& b) const {
        return !cmp_(a, b) && !cmp_(b, a);
    }

    uint32_t next_rand() {
        rng_ ^= rng_ << 13;
        rng_ ^= rng_ >> 17;
        rng_ ^= rng_ << 5;
        return rng_;
    }

    int random_level() {
        int lvl = 0;
        while (lvl < MAX_LEVEL && (next_rand() & 1)) ++lvl;
        return lvl;
    }
};

#endif // SKIP_LIST_H
