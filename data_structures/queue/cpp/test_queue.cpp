#include "queue.h"
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
    { Queue q; check("new is empty", q.is_empty()); }
    { Queue q; check("new size 0", q.size() == 0); }

    {
        Queue q;
        q.enqueue(1); q.enqueue(2); q.enqueue(3);
        check("FIFO dequeue1", q.dequeue() == 1);
        check("FIFO dequeue2", q.dequeue() == 2);
        check("FIFO dequeue3", q.dequeue() == 3);
        check("empty after all dequeue", q.is_empty());
    }

    {
        Queue q;
        q.enqueue(5);
        check("peek front", q.peek() == 5);
        check("peek no remove", q.size() == 1);
    }

    {
        Queue q;
        try { q.dequeue(); fail("dequeue empty throws", "no exception"); }
        catch (const std::underflow_error&) { pass("dequeue empty throws"); }
    }

    {
        Queue q;
        for (int i = 0; i < 100; ++i) q.enqueue(i);
        check("size 100", q.size() == 100);
        for (int i = 0; i < 100; ++i)
            if (q.dequeue() != i) { fail("large FIFO order", "mismatch at " + std::to_string(i)); break; }
        check("empty after 100", q.is_empty());
    }

    {
        Queue q;
        q.enqueue(1); q.dequeue();
        q.enqueue(2); q.dequeue();
        q.enqueue(3);
        check("wrap-around peek", q.peek() == 3);
        check("wrap-around size", q.size() == 1);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
