#include "trie.h"

Trie::Trie() : root_(new Node{}) {}

Trie::~Trie() { destroy(root_); }

void Trie::destroy(Node* n) {
    if (!n) return;
    for (auto* c : n->children) destroy(c);
    delete n;
}

void Trie::insert(const std::string& word) {
    Node* cur = root_;
    for (char ch : word) {
        int idx = ch - 'a';
        if (!cur->children[idx]) cur->children[idx] = new Node{};
        cur = cur->children[idx];
        ++cur->count;
    }
    if (!cur->is_end) { cur->is_end = true; ++size_; }
}

bool Trie::search(const std::string& word) const {
    Node* cur = root_;
    for (char ch : word) {
        int idx = ch - 'a';
        if (!cur->children[idx]) return false;
        cur = cur->children[idx];
    }
    return cur->is_end;
}

bool Trie::starts_with(const std::string& prefix) const {
    Node* cur = root_;
    for (char ch : prefix) {
        int idx = ch - 'a';
        if (!cur->children[idx]) return false;
        cur = cur->children[idx];
    }
    return true;
}

bool Trie::remove(Node* n, const std::string& word, int depth, bool& removed) {
    if (!n) return false;
    if (depth == static_cast<int>(word.size())) {
        if (!n->is_end) return false;
        n->is_end = false;
        removed = true;
        return n->count == 0;
    }
    int idx = word[depth] - 'a';
    if (!n->children[idx]) return false;
    bool should_delete = remove(n->children[idx], word, depth + 1, removed);
    if (removed) --n->children[idx]->count;
    if (should_delete) {
        delete n->children[idx];
        n->children[idx] = nullptr;
    }
    return !n->is_end && n->count == 0;
}

bool Trie::remove(const std::string& word) {
    bool removed = false;
    remove(root_, word, 0, removed);
    if (removed) --size_;
    return removed;
}

int Trie::size() const { return size_; }
