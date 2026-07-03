#include "deque.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

namespace {
bool allPassed = true;
void pass(const std::string& n) { std::cout << "[PASS] " << n << "\n"; }
void fail(const std::string& n, const std::string& d) {
    std::cerr << "[FAIL] " << n << ": " << d << "\n"; allPassed = false;
}
void check(const std::string& n, bool c, const std::string& d = "assertion failed") {
    if (c) pass(n); else fail(n, d);
}
} // namespace

int main() {
    { Deque d; check("new is empty", d.is_empty()); }

    {
        Deque d;
        d.push_back(1); d.push_back(2); d.push_back(3);
        check("push_back/peek_back", d.peek_back() == 3);
        check("push_back/peek_front", d.peek_front() == 1);
    }

    {
        Deque d;
        d.push_front(3); d.push_front(2); d.push_front(1);
        check("push_front FIFO front", d.pop_front() == 1);
        check("push_front FIFO front2", d.pop_front() == 2);
        check("push_front FIFO front3", d.pop_front() == 3);
    }

    {
        Deque d;
        d.push_back(10); d.push_back(20);
        check("pop_back LIFO", d.pop_back() == 20);
        check("pop_back LIFO2", d.pop_back() == 10);
    }

    {
        Deque d;
        d.push_front(2); d.push_front(1); d.push_back(3); d.push_back(4);
        check("mixed size", d.size() == 4);
        check("mixed front", d.pop_front() == 1);
        check("mixed back", d.pop_back() == 4);
    }

    {
        Deque d;
        try { d.pop_front(); fail("pop_front empty", "no exception"); }
        catch (const std::underflow_error&) { pass("pop_front empty throws"); }
        try { d.pop_back(); fail("pop_back empty", "no exception"); }
        catch (const std::underflow_error&) { pass("pop_back empty throws"); }
    }

    {
        Deque d;
        for (int i = 0; i < 50; ++i) { d.push_front(i); d.push_back(100 + i); }
        check("large size", d.size() == 100);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
