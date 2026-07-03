#include "queue.h"

Queue::Queue() : buf_(4), head_(0), count_(0) {}

void Queue::grow() {
    int newCap = static_cast<int>(buf_.size()) * 2;
    std::vector<int> nb(newCap);
    for (int i = 0; i < count_; ++i)
        nb[i] = buf_[(head_ + i) % static_cast<int>(buf_.size())];
    buf_ = std::move(nb);
    head_ = 0;
}

void Queue::enqueue(int value) {
    if (count_ == static_cast<int>(buf_.size())) grow();
    int tail = (head_ + count_) % static_cast<int>(buf_.size());
    buf_[tail] = value;
    ++count_;
}

int Queue::dequeue() {
    if (count_ == 0) throw std::underflow_error("queue is empty");
    int v = buf_[head_];
    head_ = (head_ + 1) % static_cast<int>(buf_.size());
    --count_;
    return v;
}

int Queue::peek() const {
    if (count_ == 0) throw std::underflow_error("queue is empty");
    return buf_[head_];
}

bool Queue::is_empty() const { return count_ == 0; }
int Queue::size() const { return count_; }
