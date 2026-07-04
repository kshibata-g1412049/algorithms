#include "hash_map.h"
#include <cstdlib>
#include <iostream>
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
    { HashMap<int, int> m; check("new is empty", m.is_empty()); }

    {
        HashMap<int, int> m;
        m.insert(1, 10); m.insert(2, 20); m.insert(3, 30);
        check("get 1",      m.get(1) == 10);
        check("get 3",      m.get(3) == 30);
        check("contains 2", m.contains(2));
        check("no 5",       !m.contains(5));
        check("size 3",     m.size() == 3);
    }

    {
        HashMap<int, int> m;
        m.insert(1, 10);
        m.insert(1, 99);
        check("update", m.get(1) == 99);
        check("size 1", m.size() == 1);
    }

    {
        HashMap<int, int> m;
        m.insert(1, 10); m.insert(2, 20);
        check("remove 1",     m.remove(1));
        check("no 1 after",   !m.contains(1));
        check("remove absent", !m.remove(99));
    }

    {
        HashMap<int, int> m;
        try { m.get(99); fail("get absent", "no exception"); }
        catch (const std::out_of_range&) { pass("get absent throws"); }
    }

    {
        HashMap<int, int> m;
        for (int i = 0; i < 100; ++i) m.insert(i, i * 2);
        bool ok = true;
        for (int i = 0; i < 100; ++i) ok = ok && (m.get(i) == i * 2);
        check("100 elements", ok);
    }

    // --- 非int型（テンプレート動作確認） ---
    {
        HashMap<std::string, std::string> m;
        m.insert("apple", "red"); m.insert("banana", "yellow");
        check("string get", m.get("apple") == "red");
        check("string contains", m.contains("banana") && !m.contains("cherry"));
        check("string remove", m.remove("apple") && m.size() == 1);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
