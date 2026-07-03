#include "trie.h"
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
    { Trie t; check("new size 0", t.size() == 0); }

    {
        Trie t;
        t.insert("apple");
        check("search apple",        t.search("apple"));
        check("no search app",       !t.search("app"));
        check("starts_with app",     t.starts_with("app"));
        check("no starts_with ban",  !t.starts_with("ban"));
        check("size 1",              t.size() == 1);
    }

    {
        Trie t;
        t.insert("apple"); t.insert("app");
        check("search app",   t.search("app"));
        check("search apple", t.search("apple"));
        check("size 2",       t.size() == 2);
        check("remove app",   t.remove("app"));
        check("apple still",  t.search("apple"));
        check("app gone",     !t.search("app"));
        check("size 1 after", t.size() == 1);
    }

    {
        Trie t;
        t.insert("cat"); t.insert("car"); t.insert("card");
        check("starts_with ca",   t.starts_with("ca"));
        check("starts_with car",  t.starts_with("car"));
        check("no starts_with cb", !t.starts_with("cb"));
        check("remove absent",    !t.remove("cap"));
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
