#ifndef AVL_TREE_H
#define AVL_TREE_H

// AVL木（自己平衡二分探索木）
//
// 操作:
//   insert(v) / remove(v) / search(v) : O(log n)
//   min() / max()                     : O(log n)
//   inorder()                         : O(n)
//   height() / size() / is_empty()    : O(1)
//
// 重複値は無視。空木での min/max は std::underflow_error。

#include <algorithm>
#include <stdexcept>
#include <vector>

class AvlTree {
public:
    AvlTree() = default;
    ~AvlTree();
    AvlTree(const AvlTree&) = delete;
    AvlTree& operator=(const AvlTree&) = delete;

    void insert(int value);
    bool remove(int value);
    bool search(int value) const;
    int min() const;
    int max() const;
    int height() const;
    int size() const;
    bool is_empty() const;
    std::vector<int> inorder() const;

private:
    struct Node {
        int val, ht;
        Node* left;
        Node* right;
    };
    Node* root_ = nullptr;
    int size_ = 0;

    void destroy(Node* n);
    int ht(Node* n) const { return n ? n->ht : 0; }
    int bf(Node* n) const { return ht(n->left) - ht(n->right); }
    void update_ht(Node* n) { n->ht = 1 + std::max(ht(n->left), ht(n->right)); }
    Node* rot_right(Node* y);
    Node* rot_left(Node* x);
    Node* balance(Node* n);
    Node* insert(Node* n, int value, bool& inserted);
    Node* remove(Node* n, int value, bool& removed);
    Node* min_node(Node* n) const;
    void inorder(Node* n, std::vector<int>& out) const;
};

#endif // AVL_TREE_H
