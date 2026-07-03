#include "circular_buffer.h"

CircularBuffer::CircularBuffer(int capacity)
    : buf_(capacity), head_(0), count_(0) {
    if (capacity <= 0) throw std::invalid_argument("capacity must be positive");
}

void CircularBuffer::push(int value) {
    if (count_ == static_cast<int>(buf_.size()))
        throw std::overflow_error("circular buffer is full");
    int tail = (head_ + count_) % static_cast<int>(buf_.size());
    buf_[tail] = value;
    ++count_;
}

int CircularBuffer::pop() {
    if (count_ == 0) throw std::underflow_error("circular buffer is empty");
    int v = buf_[head_];
    head_ = (head_ + 1) % static_cast<int>(buf_.size());
    --count_;
    return v;
}

int CircularBuffer::peek() const {
    if (count_ == 0) throw std::underflow_error("circular buffer is empty");
    return buf_[head_];
}

bool CircularBuffer::is_empty() const { return count_ == 0; }
bool CircularBuffer::is_full() const { return count_ == static_cast<int>(buf_.size()); }
int CircularBuffer::size() const { return count_; }
int CircularBuffer::capacity() const { return static_cast<int>(buf_.size()); }
