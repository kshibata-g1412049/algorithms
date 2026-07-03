#include "circular_buffer.h"
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
    { CircularBuffer b(3); check("new is empty", b.is_empty()); }
    { CircularBuffer b(3); check("new size 0", b.size() == 0); }
    { CircularBuffer b(3); check("capacity 3", b.capacity() == 3); }

    {
        CircularBuffer b(3);
        b.push(1); b.push(2); b.push(3);
        check("full after 3 pushes", b.is_full());
        check("peek front", b.peek() == 1);
    }

    {
        CircularBuffer b(3);
        b.push(1); b.push(2); b.push(3);
        check("pop FIFO 1", b.pop() == 1);
        check("pop FIFO 2", b.pop() == 2);
        b.push(4);
        check("wrap-around push", b.peek() == 3);
        check("size after wrap", b.size() == 2);
    }

    {
        CircularBuffer b(2);
        b.push(1); b.push(2);
        try { b.push(3); fail("push full throws", "no exception"); }
        catch (const std::overflow_error&) { pass("push full throws"); }
    }

    {
        CircularBuffer b(2);
        try { b.pop(); fail("pop empty throws", "no exception"); }
        catch (const std::underflow_error&) { pass("pop empty throws"); }
    }

    {
        try { CircularBuffer b(0); fail("zero capacity throws", "no exception"); }
        catch (const std::invalid_argument&) { pass("zero capacity throws"); }
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
