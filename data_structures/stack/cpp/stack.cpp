#include "stack.h"

void Stack::push(int value) {
    data_.push_back(value);
}

int Stack::pop() {
    if (data_.empty()) throw std::underflow_error("stack is empty");
    int v = data_.back();
    data_.pop_back();
    return v;
}

int Stack::peek() const {
    if (data_.empty()) throw std::underflow_error("stack is empty");
    return data_.back();
}

bool Stack::is_empty() const { return data_.empty(); }

int Stack::size() const { return static_cast<int>(data_.size()); }
