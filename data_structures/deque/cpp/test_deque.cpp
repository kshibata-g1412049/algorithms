#include "deque.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

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
    { Deque<int> d; check("new is empty", d.is_empty()); }

    {
        Deque<int> d;
        d.push_back(1); d.push_back(2); d.push_back(3);
        check("push_back/peek_back", d.peek_back() == 3);
        check("push_back/peek_front", d.peek_front() == 1);
    }

    {
        Deque<int> d;
        d.push_front(3); d.push_front(2); d.push_front(1);
        check("push_front FIFO front", d.pop_front() == 1);
        check("push_front FIFO front2", d.pop_front() == 2);
        check("push_front FIFO front3", d.pop_front() == 3);
    }

    {
        Deque<int> d;
        d.push_back(10); d.push_back(20);
        check("pop_back LIFO", d.pop_back() == 20);
        check("pop_back LIFO2", d.pop_back() == 10);
    }

    {
        Deque<int> d;
        d.push_front(2); d.push_front(1); d.push_back(3); d.push_back(4);
        check("mixed size", d.size() == 4);
        check("mixed front", d.pop_front() == 1);
        check("mixed back", d.pop_back() == 4);
    }

    {
        Deque<int> d;
        try { d.pop_front(); fail("pop_front empty", "no exception"); }
        catch (const std::underflow_error&) { pass("pop_front empty throws"); }
        try { d.pop_back(); fail("pop_back empty", "no exception"); }
        catch (const std::underflow_error&) { pass("pop_back empty throws"); }
    }

    {
        Deque<int> d;
        for (int i = 0; i < 50; ++i) { d.push_front(i); d.push_back(100 + i); }
        check("large size", d.size() == 100);
    }

    // --- 非int型（テンプレート動作確認） ---
    {
        Deque<std::string> d;
        d.push_back("mid"); d.push_front("front"); d.push_back("back");
        check("string front", d.pop_front() == "front");
        check("string back", d.pop_back() == "back");
        check("string mid", d.pop_front() == "mid");
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
