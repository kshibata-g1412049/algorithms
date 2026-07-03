#include "hash_set.h"

HashSet::HashSet() : table_(16, {0, State::EMPTY}) {}

int HashSet::probe(int value) const {
    unsigned int h = static_cast<unsigned int>(value) * 2654435761u;
    int i = static_cast<int>(h % static_cast<unsigned int>(capacity_));
    while (table_[i].state == State::USED && table_[i].value != value)
        i = (i + 1) % capacity_;
    return i;
}

void HashSet::rehash() {
    std::vector<Slot> old = std::move(table_);
    capacity_ *= 2;
    table_.assign(capacity_, {0, State::EMPTY});
    size_ = 0;
    for (auto& s : old)
        if (s.state == State::USED) insert(s.value);
}

void HashSet::insert(int value) {
    if (static_cast<double>(size_ + 1) / capacity_ > LOAD_FACTOR) rehash();
    int i = probe(value);
    if (table_[i].state != State::USED) {
        table_[i] = {value, State::USED};
        ++size_;
    }
}

bool HashSet::contains(int value) const {
    int i = probe(value);
    return table_[i].state == State::USED && table_[i].value == value;
}

bool HashSet::remove(int value) {
    int i = probe(value);
    if (table_[i].state != State::USED || table_[i].value != value) return false;
    table_[i].state = State::DELETED;
    --size_;
    return true;
}

int HashSet::size() const { return size_; }
bool HashSet::is_empty() const { return size_ == 0; }
