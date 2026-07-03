#include "avl_tree.h"

AvlTree::~AvlTree() { destroy(root_); }

void AvlTree::destroy(Node* n) {
    if (!n) return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}

AvlTree::Node* AvlTree::rot_right(Node* y) {
    Node* x = y->left;
    y->left  = x->right;
    x->right = y;
    update_ht(y);
    update_ht(x);
    return x;
}

AvlTree::Node* AvlTree::rot_left(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    y->left  = x;
    update_ht(x);
    update_ht(y);
    return y;
}

AvlTree::Node* AvlTree::balance(Node* n) {
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

AvlTree::Node* AvlTree::insert(Node* n, int value, bool& inserted) {
    if (!n) {
        inserted = true;
        return new Node{value, 1, nullptr, nullptr};
    }
    if (value < n->val)      n->left  = insert(n->left,  value, inserted);
    else if (value > n->val) n->right = insert(n->right, value, inserted);
    return balance(n);
}

void AvlTree::insert(int value) {
    bool inserted = false;
    root_ = insert(root_, value, inserted);
    if (inserted) ++size_;
}

AvlTree::Node* AvlTree::min_node(Node* n) const {
    while (n->left) n = n->left;
    return n;
}

AvlTree::Node* AvlTree::remove(Node* n, int value, bool& removed) {
    if (!n) return nullptr;
    if (value < n->val) {
        n->left  = remove(n->left,  value, removed);
    } else if (value > n->val) {
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

bool AvlTree::remove(int value) {
    bool removed = false;
    root_ = remove(root_, value, removed);
    if (removed) --size_;
    return removed;
}

bool AvlTree::search(int value) const {
    Node* n = root_;
    while (n) {
        if (value == n->val) return true;
        n = value < n->val ? n->left : n->right;
    }
    return false;
}

int AvlTree::min() const {
    if (!root_) throw std::underflow_error("tree is empty");
    return min_node(root_)->val;
}

int AvlTree::max() const {
    if (!root_) throw std::underflow_error("tree is empty");
    Node* cur = root_;
    while (cur->right) cur = cur->right;
    return cur->val;
}

int AvlTree::height() const { return ht(root_); }
int AvlTree::size() const { return size_; }
bool AvlTree::is_empty() const { return size_ == 0; }

void AvlTree::inorder(Node* n, std::vector<int>& out) const {
    if (!n) return;
    inorder(n->left, out);
    out.push_back(n->val);
    inorder(n->right, out);
}

std::vector<int> AvlTree::inorder() const {
    std::vector<int> v;
    inorder(root_, v);
    return v;
}
