#include "deque.h"

Deque::Deque() : buf_(4), head_(0), count_(0) {}

void Deque::grow() {
    int newCap = cap() * 2;
    std::vector<int> nb(newCap);
    for (int i = 0; i < count_; ++i)
        nb[i] = buf_[(head_ + i) % cap()];
    buf_ = std::move(nb);
    head_ = 0;
}

void Deque::push_front(int value) {
    if (count_ == cap()) grow();
    head_ = (head_ - 1 + cap()) % cap();
    buf_[head_] = value;
    ++count_;
}

void Deque::push_back(int value) {
    if (count_ == cap()) grow();
    buf_[(head_ + count_) % cap()] = value;
    ++count_;
}

int Deque::pop_front() {
    if (count_ == 0) throw std::underflow_error("deque is empty");
    int v = buf_[head_];
    head_ = (head_ + 1) % cap();
    --count_;
    return v;
}

int Deque::pop_back() {
    if (count_ == 0) throw std::underflow_error("deque is empty");
    int idx = (head_ + count_ - 1) % cap();
    --count_;
    return buf_[idx];
}

int Deque::peek_front() const {
    if (count_ == 0) throw std::underflow_error("deque is empty");
    return buf_[head_];
}

int Deque::peek_back() const {
    if (count_ == 0) throw std::underflow_error("deque is empty");
    return buf_[(head_ + count_ - 1) % cap()];
}

bool Deque::is_empty() const { return count_ == 0; }
int Deque::size() const { return count_; }
