#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

// 単方向連結リスト（テンプレート版）
//
// 任意の型 T を要素として格納できる（T は == 比較可能であること）。
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

template <typename T>
class SinglyLinkedList {
public:
    SinglyLinkedList() = default;
    ~SinglyLinkedList() {
        while (head_) { Node* n = head_->next; delete head_; head_ = n; }
    }
    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;

    void push_front(const T& value) {
        head_ = new Node{value, head_};
        ++size_;
    }

    void push_back(const T& value) {
        Node* n = new Node{value, nullptr};
        if (!head_) { head_ = n; }
        else { Node* cur = head_; while (cur->next) cur = cur->next; cur->next = n; }
        ++size_;
    }

    T pop_front() {
        if (!head_) throw std::underflow_error("list is empty");
        T v = head_->val;
        Node* n = head_->next;
        delete head_;
        head_ = n;
        --size_;
        return v;
    }

    const T& front() const {
        if (!head_) throw std::underflow_error("list is empty");
        return head_->val;
    }

    const T& back() const {
        if (!head_) throw std::underflow_error("list is empty");
        Node* cur = head_;
        while (cur->next) cur = cur->next;
        return cur->val;
    }

    bool contains(const T& value) const {
        for (Node* cur = head_; cur; cur = cur->next)
            if (cur->val == value) return true;
        return false;
    }

    bool remove(const T& value) {
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

    int size() const { return size_; }
    bool is_empty() const { return size_ == 0; }

    std::vector<T> to_vector() const {
        std::vector<T> v;
        for (Node* cur = head_; cur; cur = cur->next) v.push_back(cur->val);
        return v;
    }

private:
    struct Node { T val; Node* next; };
    Node* head_ = nullptr;
    int size_ = 0;
};

#endif // SINGLY_LINKED_LIST_H
