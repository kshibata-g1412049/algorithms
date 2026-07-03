#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

// 単方向連結リスト
//
// 操作:
//   push_front(v)       : 先頭に挿入  O(1)
//   push_back(v)        : 末尾に追加  O(n)
//   pop_front()         : 先頭を取り出す  O(1)
//   front() / back()    : 先頭/末尾の参照  O(1)/O(n)
//   contains(v)         : 値を検索  O(n)
//   remove(v)           : 最初に見つかった値を削除  O(n)
//   size() / is_empty() : O(1)
//   to_vector()         : 先頭→末尾のコピー  O(n)
//
// 空状態での front/back/pop_front は std::underflow_error を投げる。

#include <stdexcept>
#include <vector>

class SinglyLinkedList {
public:
    SinglyLinkedList() = default;
    ~SinglyLinkedList();
    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;

    void push_front(int value);
    void push_back(int value);
    int pop_front();
    int front() const;
    int back() const;
    bool contains(int value) const;
    bool remove(int value);
    int size() const;
    bool is_empty() const;
    std::vector<int> to_vector() const;

private:
    struct Node { int val; Node* next; };
    Node* head_ = nullptr;
    int size_ = 0;
};

#endif // SINGLY_LINKED_LIST_H
