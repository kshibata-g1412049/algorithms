#ifndef AVL_TREE_H
#define AVL_TREE_H

// AVL木（自己平衡二分探索木）（テンプレート版）
//
// 任意の型 T を格納できる。順序は Compare（既定 std::less<T>）で決まる。
//
// 操作:
//   insert(v) / remove(v) / search(v) : O(log n)
//   min() / max()                     : O(log n)
//   inorder()                         : O(n)
//   height() / size() / is_empty()    : O(1)
//
// 重複値は無視。空木での min/max は std::underflow_error。

#include <algorithm>
#include <functional>
#include <stdexcept>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class AvlTree {
public:
    AvlTree() = default;
    ~AvlTree() { destroy(root_); }
    AvlTree(const AvlTree&) = delete;
    AvlTree& operator=(const AvlTree&) = delete;

    void insert(const T& value) {
        bool inserted = false;
        root_ = insert(root_, value, inserted);
        if (inserted) ++size_;
    }

    bool remove(const T& value) {
        bool removed = false;
        root_ = remove(root_, value, removed);
        if (removed) --size_;
        return removed;
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

    int height() const { return ht(root_); }
    int size() const { return size_; }
    bool is_empty() const { return size_ == 0; }

    std::vector<T> inorder() const {
        std::vector<T> v;
        inorder(root_, v);
        return v;
    }

private:
    struct Node {
        T val;
        int ht;
        Node* left;
        Node* right;
    };
    Node* root_ = nullptr;
    int size_ = 0;
    Compare cmp_;

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    int ht(Node* n) const { return n ? n->ht : 0; }
    int bf(Node* n) const { return ht(n->left) - ht(n->right); }
    void update_ht(Node* n) { n->ht = 1 + std::max(ht(n->left), ht(n->right)); }

    Node* rot_right(Node* y) {
        Node* x = y->left;
        y->left  = x->right;
        x->right = y;
        update_ht(y);
        update_ht(x);
        return x;
    }

    Node* rot_left(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left  = x;
        update_ht(x);
        update_ht(y);
        return y;
    }

    Node* balance(Node* n) {
        update_ht(n);
        int b = bf(n);
        if (b > 1) {
            if (bf(n->left) < 0) n->left = rot_left(n->left);
            return rot_right(n);
        }
        if (b < -1) {
            if (bf(n->right) > 0) n->right = rot_right(n->right);
            return rot_left(n);
        }
        return n;
    }

    Node* insert(Node* n, const T& value, bool& inserted) {
        if (!n) {
            inserted = true;
            return new Node{value, 1, nullptr, nullptr};
        }
        if (cmp_(value, n->val))      n->left  = insert(n->left,  value, inserted);
        else if (cmp_(n->val, value)) n->right = insert(n->right, value, inserted);
        return balance(n);
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
            if (!n->left || !n->right) {
                Node* child = n->left ? n->left : n->right;
                delete n;
                return child;
            }
            Node* succ = min_node(n->right);
            n->val = succ->val;
            bool dummy = false;
            n->right = remove(n->right, succ->val, dummy);
        }
        return balance(n);
    }

    void inorder(Node* n, std::vector<T>& out) const {
        if (!n) return;
        inorder(n->left, out);
        out.push_back(n->val);
        inorder(n->right, out);
    }
};

#endif // AVL_TREE_H
