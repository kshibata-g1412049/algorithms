#include "doubly_linked_list.h"

DoublyLinkedList::~DoublyLinkedList() {
    while (head_) { Node* n = head_->next; delete head_; head_ = n; }
}

void DoublyLinkedList::unlink(Node* n) {
    if (n->prev) n->prev->next = n->next; else head_ = n->next;
    if (n->next) n->next->prev = n->prev; else tail_ = n->prev;
    delete n;
    --size_;
}

void DoublyLinkedList::push_front(int value) {
    Node* n = new Node{value, nullptr, head_};
    if (head_) head_->prev = n; else tail_ = n;
    head_ = n;
    ++size_;
}

void DoublyLinkedList::push_back(int value) {
    Node* n = new Node{value, tail_, nullptr};
    if (tail_) tail_->next = n; else head_ = n;
    tail_ = n;
    ++size_;
}

int DoublyLinkedList::pop_front() {
    if (!head_) throw std::underflow_error("list is empty");
    int v = head_->val;
    unlink(head_);
    return v;
}

int DoublyLinkedList::pop_back() {
    if (!tail_) throw std::underflow_error("list is empty");
    int v = tail_->val;
    unlink(tail_);
    return v;
}

int DoublyLinkedList::front() const {
    if (!head_) throw std::underflow_error("list is empty");
    return head_->val;
}

int DoublyLinkedList::back() const {
    if (!tail_) throw std::underflow_error("list is empty");
    return tail_->val;
}

bool DoublyLinkedList::contains(int value) const {
    for (Node* cur = head_; cur; cur = cur->next)
        if (cur->val == value) return true;
    return false;
}

bool DoublyLinkedList::remove(int value) {
    for (Node* cur = head_; cur; cur = cur->next) {
        if (cur->val == value) { unlink(cur); return true; }
    }
    return false;
}

int DoublyLinkedList::size() const { return size_; }
bool DoublyLinkedList::is_empty() const { return size_ == 0; }

std::vector<int> DoublyLinkedList::to_vector() const {
    std::vector<int> v;
    for (Node* cur = head_; cur; cur = cur->next) v.push_back(cur->val);
    return v;
}
