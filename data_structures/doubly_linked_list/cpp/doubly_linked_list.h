#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

// 双方向連結リスト
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

class DoublyLinkedList {
public:
    DoublyLinkedList() = default;
    ~DoublyLinkedList();
    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    void push_front(int value);
    void push_back(int value);
    int pop_front();
    int pop_back();
    int front() const;
    int back() const;
    bool contains(int value) const;
    bool remove(int value);
    int size() const;
    bool is_empty() const;
    std::vector<int> to_vector() const;

private:
    struct Node { int val; Node* prev; Node* next; };
    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    int size_ = 0;

    void unlink(Node* n);
};

#endif // DOUBLY_LINKED_LIST_H
