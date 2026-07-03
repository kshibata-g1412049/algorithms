#include "binary_search_tree.h"
#include <cstdlib>
#include <iostream>
#include <vector>

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
    { BinarySearchTree t; check("new is empty", t.is_empty()); }

    {
        BinarySearchTree t;
        t.insert(5); t.insert(3); t.insert(7); t.insert(1); t.insert(4);
        check("search 3",   t.search(3));
        check("search 7",   t.search(7));
        check("no search 6", !t.search(6));
        check("size 5", t.size() == 5);
    }

    {
        BinarySearchTree t;
        t.insert(5); t.insert(3); t.insert(7); t.insert(1); t.insert(4);
        auto io = t.inorder();
        check("inorder sorted", io == std::vector<int>{1, 3, 4, 5, 7});
    }

    {
        BinarySearchTree t;
        t.insert(5); t.insert(3); t.insert(7);
        check("min", t.min() == 3);
        check("max", t.max() == 7);
    }

    {
        BinarySearchTree t;
        t.insert(5); t.insert(3); t.insert(7);
        check("remove leaf",  t.remove(3));
        check("after remove", t.inorder() == std::vector<int>{5, 7});
        check("remove root",  t.remove(5));
        check("new root",     t.inorder() == std::vector<int>{7});
        check("remove absent", !t.remove(99));
    }

    {
        BinarySearchTree t;
        check("duplicate ignored", [&]{
            t.insert(5); t.insert(5);
            return t.size() == 1;
        }());
    }

    {
        BinarySearchTree t;
        try { t.min(); fail("min empty", "no exception"); }
        catch (const std::underflow_error&) { pass("min empty throws"); }
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
