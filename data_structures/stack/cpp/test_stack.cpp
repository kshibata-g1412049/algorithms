#include "stack.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {
bool allPassed = true;

void pass(const std::string& name) { std::cout << "[PASS] " << name << "\n"; }
void fail(const std::string& name, const std::string& detail) {
    std::cerr << "[FAIL] " << name << ": " << detail << "\n";
    allPassed = false;
}

void check(const std::string& name, bool cond, const std::string& detail = "assertion failed") {
    if (cond) pass(name); else fail(name, detail);
}
} // namespace

int main() {
    { Stack<int> s; check("new stack is empty", s.is_empty()); }
    { Stack<int> s; check("new stack size 0", s.size() == 0); }

    {
        Stack<int> s;
        s.push(10);
        check("push/peek", s.peek() == 10);
        check("size after push", s.size() == 1);
        check("not empty after push", !s.is_empty());
    }

    {
        Stack<int> s;
        s.push(1); s.push(2); s.push(3);
        check("LIFO order pop1", s.pop() == 3);
        check("LIFO order pop2", s.pop() == 2);
        check("LIFO order pop3", s.pop() == 1);
        check("empty after all pops", s.is_empty());
    }

    {
        Stack<int> s;
        s.push(5); s.push(7);
        check("peek does not pop", s.peek() == 7);
        check("size unchanged after peek", s.size() == 2);
    }

    {
        Stack<int> s;
        try { s.pop(); fail("pop empty throws", "no exception"); }
        catch (const std::underflow_error&) { pass("pop empty throws"); }
    }

    {
        Stack<int> s;
        try { s.peek(); fail("peek empty throws", "no exception"); }
        catch (const std::underflow_error&) { pass("peek empty throws"); }
    }

    {
        Stack<int> s;
        for (int i = 0; i < 100; ++i) s.push(i);
        check("size 100", s.size() == 100);
        check("top after 100 pushes", s.peek() == 99);
        for (int i = 99; i >= 0; --i) if (s.pop() != i) { fail("large pop order", "mismatch"); break; }
        if (s.is_empty()) pass("empty after 100 pops");
    }

    // --- 非int型（テンプレート動作確認） ---
    {
        Stack<std::string> s;
        s.push("hello"); s.push("world");
        check("string LIFO", s.pop() == "world" && s.pop() == "hello");
    }
    {
        Stack<double> s;
        s.push(3.14); s.push(2.71);
        check("double pop", s.pop() == 2.71);
        check("double peek", s.peek() == 3.14);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
