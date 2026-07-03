#ifndef TRIE_H
#define TRIE_H

// トライ木（前置木）
//
// 操作:
//   insert(word)       : 文字列を挿入  O(L)
//   search(word)       : 文字列を検索  O(L)
//   starts_with(prefix): プレフィックス検索  O(L)
//   remove(word)       : 文字列を削除  O(L)
//   size()             : 挿入済み単語数  O(1)
//
// 小文字英字 a-z のみ対応。

#include <string>

class Trie {
public:
    Trie();
    ~Trie();
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;

    void insert(const std::string& word);
    bool search(const std::string& word) const;
    bool starts_with(const std::string& prefix) const;
    bool remove(const std::string& word);
    int size() const;

private:
    struct Node {
        Node* children[26] = {};
        bool is_end = false;
        int count = 0;
    };
    Node* root_;
    int size_ = 0;

    void destroy(Node* n);
    bool remove(Node* n, const std::string& word, int depth, bool& removed);
};

#endif // TRIE_H
