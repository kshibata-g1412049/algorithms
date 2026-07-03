#include "hash_set.h"
#include <cstdlib>
#include <iostream>

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
    { HashSet s; check("new is empty", s.is_empty()); }

    {
        HashSet s;
        s.insert(1); s.insert(2); s.insert(3);
        check("contains 1",   s.contains(1));
        check("no contains 5", !s.contains(5));
        check("size 3",       s.size() == 3);
    }

    {
        HashSet s;
        s.insert(1); s.insert(1);
        check("no duplicate", s.size() == 1);
    }

    {
        HashSet s;
        s.insert(1); s.insert(2); s.insert(3);
        check("remove 2",    s.remove(2));
        check("no 2 after",  !s.contains(2));
        check("1 still",     s.contains(1));
        check("remove abs",  !s.remove(99));
        check("size 2",      s.size() == 2);
    }

    {
        HashSet s;
        for (int i = 0; i < 100; ++i) s.insert(i);
        bool ok = true;
        for (int i = 0; i < 100; ++i) ok = ok && s.contains(i);
        check("100 elements", ok);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
