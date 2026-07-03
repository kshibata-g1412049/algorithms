#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

// 二分探索木 (BST)
//
// 操作:
//   insert(v)              : 挿入  O(h)
//   search(v)              : 検索  O(h)
//   remove(v)              : 削除  O(h)
//   min() / max()          : 最小/最大値  O(h)
//   inorder/preorder/postorder : 走査  O(n)
//   size() / is_empty()    : O(1)
//
// 平均 h = O(log n)、最悪 h = O(n)（偏り木）。
// 空木での min/max は std::underflow_error を投げる。
// 重複値は無視（挿入しない）。

#include <functional>
#include <stdexcept>
#include <vector>

class BinarySearchTree {
public:
    BinarySearchTree() = default;
    ~BinarySearchTree();
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    void insert(int value);
    bool search(int value) const;
    bool remove(int value);
    int min() const;
    int max() const;
    int size() const;
    bool is_empty() const;

    std::vector<int> inorder() const;
    std::vector<int> preorder() const;
    std::vector<int> postorder() const;

private:
    struct Node { int val; Node* left; Node* right; };
    Node* root_ = nullptr;
    int size_ = 0;

    void destroy(Node* n);
    Node* insert(Node* n, int value, bool& inserted);
    bool search(Node* n, int value) const;
    Node* remove(Node* n, int value, bool& removed);
    Node* min_node(Node* n) const;
    void inorder(Node* n, std::vector<int>& out) const;
    void preorder(Node* n, std::vector<int>& out) const;
    void postorder(Node* n, std::vector<int>& out) const;
};

#endif // BINARY_SEARCH_TREE_H
