#include "binary_search_tree.h"
#include <cstdlib>
#include <iostream>
#include <vector>
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
    { BinarySearchTree<int> t; check("new is empty", t.is_empty()); }

    {
        BinarySearchTree<int> t;
        t.insert(5); t.insert(3); t.insert(7); t.insert(1); t.insert(4);
        check("search 3",   t.search(3));
        check("search 7",   t.search(7));
        check("no search 6", !t.search(6));
        check("size 5", t.size() == 5);
    }

    {
        BinarySearchTree<int> t;
        t.insert(5); t.insert(3); t.insert(7); t.insert(1); t.insert(4);
        auto io = t.inorder();
        check("inorder sorted", io == std::vector<int>{1, 3, 4, 5, 7});
    }

    {
        BinarySearchTree<int> t;
        t.insert(5); t.insert(3); t.insert(7);
        check("min", t.min() == 3);
        check("max", t.max() == 7);
    }

    {
        BinarySearchTree<int> t;
        t.insert(5); t.insert(3); t.insert(7);
        check("remove leaf",  t.remove(3));
        check("after remove", t.inorder() == std::vector<int>{5, 7});
        check("remove root",  t.remove(5));
        check("new root",     t.inorder() == std::vector<int>{7});
        check("remove absent", !t.remove(99));
    }

    {
        BinarySearchTree<int> t;
        check("duplicate ignored", [&]{
            t.insert(5); t.insert(5);
            return t.size() == 1;
        }());
    }

    {
        BinarySearchTree<int> t;
        try { t.min(); fail("min empty", "no exception"); }
        catch (const std::underflow_error&) { pass("min empty throws"); }
    }

    // --- 非int型（テンプレート動作確認） ---
    {
        BinarySearchTree<std::string> t;
        t.insert("banana"); t.insert("apple"); t.insert("cherry");
        check("string search", t.search("apple") && !t.search("durian"));
        check("string min/max", t.min() == "apple" && t.max() == "cherry");
        std::vector<std::string> in = t.inorder();
        check("string inorder sorted",
              in == std::vector<std::string>({"apple", "banana", "cherry"}));
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
