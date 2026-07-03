#include "skip_list.h"

SkipList::SkipList(uint32_t seed)
    : head_(new Node(INT32_MIN, MAX_LEVEL)), rng_(seed) {}

SkipList::~SkipList() {
    Node* cur = head_;
    while (cur) { Node* n = cur->forward[0]; delete cur; cur = n; }
}

uint32_t SkipList::next_rand() {
    rng_ ^= rng_ << 13;
    rng_ ^= rng_ >> 17;
    rng_ ^= rng_ << 5;
    return rng_;
}

int SkipList::random_level() {
    int lvl = 0;
    while (lvl < MAX_LEVEL && (next_rand() & 1)) ++lvl;
    return lvl;
}

void SkipList::insert(int value) {
    std::vector<Node*> update(MAX_LEVEL + 1);
    Node* cur = head_;
    for (int i = level_; i >= 0; --i) {
        while (cur->forward[i] && cur->forward[i]->val < value)
            cur = cur->forward[i];
        update[i] = cur;
    }
    cur = cur->forward[0];
    if (cur && cur->val == value) return;
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

bool SkipList::search(int value) const {
    Node* cur = head_;
    for (int i = level_; i >= 0; --i)
        while (cur->forward[i] && cur->forward[i]->val < value)
            cur = cur->forward[i];
    cur = cur->forward[0];
    return cur && cur->val == value;
}

bool SkipList::remove(int value) {
    std::vector<Node*> update(MAX_LEVEL + 1);
    Node* cur = head_;
    for (int i = level_; i >= 0; --i) {
        while (cur->forward[i] && cur->forward[i]->val < value)
            cur = cur->forward[i];
        update[i] = cur;
    }
    cur = cur->forward[0];
    if (!cur || cur->val != value) return false;
    for (int i = 0; i <= level_; ++i) {
        if (update[i]->forward[i] != cur) break;
        update[i]->forward[i] = cur->forward[i];
    }
    delete cur;
    while (level_ > 0 && !head_->forward[level_]) --level_;
    --size_;
    return true;
}

int SkipList::size() const { return size_; }
bool SkipList::is_empty() const { return size_ == 0; }
