#ifndef SKIP_LIST_H
#define SKIP_LIST_H

// スキップリスト（確率的データ構造）
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
#include <vector>

class SkipList {
public:
    explicit SkipList(uint32_t seed = 42);
    ~SkipList();
    SkipList(const SkipList&) = delete;
    SkipList& operator=(const SkipList&) = delete;

    void insert(int value);
    bool search(int value) const;
    bool remove(int value);
    int size() const;
    bool is_empty() const;

private:
    static constexpr int MAX_LEVEL = 16;

    struct Node {
        int val;
        std::vector<Node*> forward;
        Node(int v, int level) : val(v), forward(level + 1, nullptr) {}
    };

    Node* head_;
    int level_ = 0;
    int size_ = 0;
    uint32_t rng_;

    int random_level();
    uint32_t next_rand();
};

#endif // SKIP_LIST_H
