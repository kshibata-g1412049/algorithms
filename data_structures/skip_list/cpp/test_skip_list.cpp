#include "skip_list.h"
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
    { SkipList sl; check("new is empty", sl.is_empty()); }

    {
        SkipList sl(42);
        sl.insert(3); sl.insert(1); sl.insert(4); sl.insert(1); sl.insert(5);
        check("search 3",    sl.search(3));
        check("search 5",    sl.search(5));
        check("no search 2", !sl.search(2));
        check("size 4",      sl.size() == 4);
    }

    {
        SkipList sl(42);
        sl.insert(3); sl.insert(1); sl.insert(4); sl.insert(5);
        check("remove 3",    sl.remove(3));
        check("no 3 after",  !sl.search(3));
        check("1 still",     sl.search(1));
        check("remove abs",  !sl.remove(99));
        check("size 3",      sl.size() == 3);
    }

    {
        SkipList sl(7);
        for (int i = 1; i <= 100; ++i) sl.insert(i);
        bool ok = true;
        for (int i = 1; i <= 100; ++i) ok = ok && sl.search(i);
        check("100 inserts", ok);
        for (int i = 1; i <= 50; ++i) sl.remove(i);
        bool removed_ok = true;
        for (int i = 1; i <= 50; ++i) removed_ok = removed_ok && !sl.search(i);
        check("50 removes",  removed_ok);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
