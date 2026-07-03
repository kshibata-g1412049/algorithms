#include "binary_search_tree.h"

BinarySearchTree::~BinarySearchTree() { destroy(root_); }

void BinarySearchTree::destroy(Node* n) {
    if (!n) return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}

BinarySearchTree::Node* BinarySearchTree::insert(Node* n, int value, bool& inserted) {
    if (!n) { inserted = true; return new Node{value, nullptr, nullptr}; }
    if (value < n->val)      n->left  = insert(n->left,  value, inserted);
    else if (value > n->val) n->right = insert(n->right, value, inserted);
    return n;
}

void BinarySearchTree::insert(int value) {
    bool inserted = false;
    root_ = insert(root_, value, inserted);
    if (inserted) ++size_;
}

bool BinarySearchTree::search(Node* n, int value) const {
    if (!n) return false;
    if (value == n->val) return true;
    return value < n->val ? search(n->left, value) : search(n->right, value);
}

bool BinarySearchTree::search(int value) const { return search(root_, value); }

BinarySearchTree::Node* BinarySearchTree::min_node(Node* n) const {
    while (n->left) n = n->left;
    return n;
}

BinarySearchTree::Node* BinarySearchTree::remove(Node* n, int value, bool& removed) {
    if (!n) return nullptr;
    if (value < n->val) {
        n->left  = remove(n->left,  value, removed);
    } else if (value > n->val) {
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

bool BinarySearchTree::remove(int value) {
    bool removed = false;
    root_ = remove(root_, value, removed);
    if (removed) --size_;
    return removed;
}

int BinarySearchTree::min() const {
    if (!root_) throw std::underflow_error("tree is empty");
    return min_node(root_)->val;
}

int BinarySearchTree::max() const {
    if (!root_) throw std::underflow_error("tree is empty");
    Node* cur = root_;
    while (cur->right) cur = cur->right;
    return cur->val;
}

int BinarySearchTree::size() const { return size_; }
bool BinarySearchTree::is_empty() const { return size_ == 0; }

void BinarySearchTree::inorder(Node* n, std::vector<int>& out) const {
    if (!n) return;
    inorder(n->left, out);
    out.push_back(n->val);
    inorder(n->right, out);
}

void BinarySearchTree::preorder(Node* n, std::vector<int>& out) const {
    if (!n) return;
    out.push_back(n->val);
    preorder(n->left, out);
    preorder(n->right, out);
}

void BinarySearchTree::postorder(Node* n, std::vector<int>& out) const {
    if (!n) return;
    postorder(n->left, out);
    postorder(n->right, out);
    out.push_back(n->val);
}

std::vector<int> BinarySearchTree::inorder()   const { std::vector<int> v; inorder(root_, v);   return v; }
std::vector<int> BinarySearchTree::preorder()  const { std::vector<int> v; preorder(root_, v);  return v; }
std::vector<int> BinarySearchTree::postorder() const { std::vector<int> v; postorder(root_, v); return v; }
