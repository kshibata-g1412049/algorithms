#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

// 二分探索木 (BST)（テンプレート版）
//
// 任意の型 T を格納できる。順序は Compare（既定 std::less<T>）で決まる。
//
// 操作:
//   insert(v)                 : 挿入  O(h)
//   search(v)                 : 検索  O(h)
//   remove(v)                 : 削除  O(h)
//   min() / max()             : 最小/最大値  O(h)
//   inorder/preorder/postorder: 走査  O(n)
//   size() / is_empty()       : O(1)
//
// 重複値は無視。空木での min/max は std::underflow_error。

#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class BinarySearchTree {
public:
    BinarySearchTree() = default;
    ~BinarySearchTree() { destroy(root_); }
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    void insert(const T& value) {
        bool inserted = false;
        root_ = insert(root_, value, inserted);
        if (inserted) ++size_;
    }

    bool search(const T& value) const {
        Node* n = root_;
        while (n) {
            if (cmp_(value, n->val))      n = n->left;
            else if (cmp_(n->val, value)) n = n->right;
            else return true;
        }
        return false;
    }

    bool remove(const T& value) {
        bool removed = false;
        root_ = remove(root_, value, removed);
        if (removed) --size_;
        return removed;
    }

    const T& min() const {
        if (!root_) throw std::underflow_error("tree is empty");
        return min_node(root_)->val;
    }

    const T& max() const {
        if (!root_) throw std::underflow_error("tree is empty");
        Node* cur = root_;
        while (cur->right) cur = cur->right;
        return cur->val;
    }

    int size() const { return size_; }
    bool is_empty() const { return size_ == 0; }

    std::vector<T> inorder()   const { std::vector<T> v; inorder(root_, v);   return v; }
    std::vector<T> preorder()  const { std::vector<T> v; preorder(root_, v);  return v; }
    std::vector<T> postorder() const { std::vector<T> v; postorder(root_, v); return v; }

private:
    struct Node { T val; Node* left; Node* right; };
    Node* root_ = nullptr;
    int size_ = 0;
    Compare cmp_;

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    Node* insert(Node* n, const T& value, bool& inserted) {
        if (!n) { inserted = true; return new Node{value, nullptr, nullptr}; }
        if (cmp_(value, n->val))      n->left  = insert(n->left,  value, inserted);
        else if (cmp_(n->val, value)) n->right = insert(n->right, value, inserted);
        return n;
    }

    Node* min_node(Node* n) const {
        while (n->left) n = n->left;
        return n;
    }

    Node* remove(Node* n, const T& value, bool& removed) {
        if (!n) return nullptr;
        if (cmp_(value, n->val)) {
            n->left  = remove(n->left,  value, removed);
        } else if (cmp_(n->val, value)) {
            n->right = remove(n->right, value, removed);
        } else {
            removed = true;
            if (!n->left)  { Node* r = n->right; delete n; return r; }
            if (!n->right) { Node* l = n->left;  delete n; return l; }
            Node* succ = min_node(n->right);
            n->val   = succ->val;
            bool dummy = false;
            n->right = remove(n->right, succ->val, dummy);
        }
        return n;
    }

    void inorder(Node* n, std::vector<T>& out) const {
        if (!n) return;
        inorder(n->left, out);
        out.push_back(n->val);
        inorder(n->right, out);
    }

    void preorder(Node* n, std::vector<T>& out) const {
        if (!n) return;
        out.push_back(n->val);
        preorder(n->left, out);
        preorder(n->right, out);
    }

    void postorder(Node* n, std::vector<T>& out) const {
        if (!n) return;
        postorder(n->left, out);
        postorder(n->right, out);
        out.push_back(n->val);
    }
};

#endif // BINARY_SEARCH_TREE_H
