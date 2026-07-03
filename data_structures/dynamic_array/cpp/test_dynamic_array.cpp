#include "dynamic_array.h"
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
    { DynamicArray a; check("new size 0", a.size() == 0); }

    {
        DynamicArray a;
        a.append(10); a.append(20); a.append(30);
        check("append get 0", a.get(0) == 10);
        check("append get 1", a.get(1) == 20);
        check("append get 2", a.get(2) == 30);
        check("size 3", a.size() == 3);
    }

    {
        DynamicArray a;
        a.append(1); a.append(2); a.append(3);
        a.set(1, 99);
        check("set", a.get(1) == 99);
    }

    {
        DynamicArray a;
        a.append(1); a.append(3);
        a.insert(1, 2);
        check("insert get 0", a.get(0) == 1);
        check("insert get 1", a.get(1) == 2);
        check("insert get 2", a.get(2) == 3);
        check("insert size", a.size() == 3);
    }

    {
        DynamicArray a;
        a.append(1); a.append(2); a.append(3);
        a.remove(1);
        check("remove get 0", a.get(0) == 1);
        check("remove get 1", a.get(1) == 3);
        check("remove size", a.size() == 2);
    }

    {
        DynamicArray a;
        try { a.get(0); fail("get empty throws", "no exception"); }
        catch (const std::out_of_range&) { pass("get empty throws"); }
    }

    {
        DynamicArray a;
        for (int i = 0; i < 100; ++i) a.append(i);
        check("large size", a.size() == 100);
        check("large get 50", a.get(50) == 50);
        check("capacity grown", a.capacity() >= 100);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
