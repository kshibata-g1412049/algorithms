#include "singly_linked_list.h"

SinglyLinkedList::~SinglyLinkedList() {
    while (head_) { Node* n = head_->next; delete head_; head_ = n; }
}

void SinglyLinkedList::push_front(int value) {
    head_ = new Node{value, head_};
    ++size_;
}

void SinglyLinkedList::push_back(int value) {
    Node* n = new Node{value, nullptr};
    if (!head_) { head_ = n; }
    else { Node* cur = head_; while (cur->next) cur = cur->next; cur->next = n; }
    ++size_;
}

int SinglyLinkedList::pop_front() {
    if (!head_) throw std::underflow_error("list is empty");
    int v = head_->val;
    Node* n = head_->next;
    delete head_;
    head_ = n;
    --size_;
    return v;
}

int SinglyLinkedList::front() const {
    if (!head_) throw std::underflow_error("list is empty");
    return head_->val;
}

int SinglyLinkedList::back() const {
    if (!head_) throw std::underflow_error("list is empty");
    Node* cur = head_;
    while (cur->next) cur = cur->next;
    return cur->val;
}

bool SinglyLinkedList::contains(int value) const {
    for (Node* cur = head_; cur; cur = cur->next)
        if (cur->val == value) return true;
    return false;
}

bool SinglyLinkedList::remove(int value) {
    if (!head_) return false;
    if (head_->val == value) { pop_front(); return true; }
    for (Node* cur = head_; cur->next; cur = cur->next) {
        if (cur->next->val == value) {
            Node* del = cur->next;
            cur->next = del->next;
            delete del;
            --size_;
            return true;
        }
    }
    return false;
}

int SinglyLinkedList::size() const { return size_; }
bool SinglyLinkedList::is_empty() const { return size_ == 0; }

std::vector<int> SinglyLinkedList::to_vector() const {
    std::vector<int> v;
    for (Node* cur = head_; cur; cur = cur->next) v.push_back(cur->val);
    return v;
}
