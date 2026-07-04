#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

// 双方向連結リスト（テンプレート版）
//
// 任意の型 T を要素として格納できる（T は == 比較可能であること）。
//
// 操作:
//   push_front(v) / push_back(v) : O(1)
//   pop_front() / pop_back()     : O(1)
//   front() / back()             : O(1)
//   contains(v) / remove(v)      : O(n)
//   size() / is_empty()          : O(1)
//   to_vector()                  : O(n)
//
// 空状態での front/back/pop_front/pop_back は std::underflow_error を投げる。

#include <stdexcept>
#include <vector>

template <typename T>
class DoublyLinkedList {
public:
    DoublyLinkedList() = default;
    ~DoublyLinkedList() {
        while (head_) { Node* n = head_->next; delete head_; head_ = n; }
    }
    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    void push_front(const T& value) {
        Node* n = new Node{value, nullptr, head_};
        if (head_) head_->prev = n; else tail_ = n;
        head_ = n;
        ++size_;
    }

    void push_back(const T& value) {
        Node* n = new Node{value, tail_, nullptr};
        if (tail_) tail_->next = n; else head_ = n;
        tail_ = n;
        ++size_;
    }

    T pop_front() {
        if (!head_) throw std::underflow_error("list is empty");
        T v = head_->val;
        unlink(head_);
        return v;
    }

    T pop_back() {
        if (!tail_) throw std::underflow_error("list is empty");
        T v = tail_->val;
        unlink(tail_);
        return v;
    }

    const T& front() const {
        if (!head_) throw std::underflow_error("list is empty");
        return head_->val;
    }

    const T& back() const {
        if (!tail_) throw std::underflow_error("list is empty");
        return tail_->val;
    }

    bool contains(const T& value) const {
        for (Node* cur = head_; cur; cur = cur->next)
            if (cur->val == value) return true;
        return false;
    }

    bool remove(const T& value) {
        for (Node* cur = head_; cur; cur = cur->next) {
            if (cur->val == value) { unlink(cur); return true; }
        }
        return false;
    }

    int size() const { return size_; }
    bool is_empty() const { return size_ == 0; }

    std::vector<T> to_vector() const {
        std::vector<T> v;
        for (Node* cur = head_; cur; cur = cur->next) v.push_back(cur->val);
        return v;
    }

private:
    struct Node { T val; Node* prev; Node* next; };
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    int size_ = 0;

    void unlink(Node* n) {
        if (n->prev) n->prev->next = n->next; else head_ = n->next;
        if (n->next) n->next->prev = n->prev; else tail_ = n->prev;
        delete n;
        --size_;
    }
};

#endif // DOUBLY_LINKED_LIST_H
