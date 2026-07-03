#include "avl_tree.h"
#include <cmath>
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
    { AvlTree t; check("new is empty", t.is_empty()); }

    {
        AvlTree t;
        for (int v : {5, 3, 7, 1, 4}) t.insert(v);
        check("search 3",    t.search(3));
        check("no search 6", !t.search(6));
        auto io = t.inorder();
        check("inorder", io == std::vector<int>{1, 3, 4, 5, 7});
    }

    {
        AvlTree t;
        for (int v : {5, 3, 7}) t.insert(v);
        check("min", t.min() == 3);
        check("max", t.max() == 7);
    }

    {
        // Ascending insert would degenerate BST but AVL keeps height O(log n)
        AvlTree t;
        for (int i = 1; i <= 100; ++i) t.insert(i);
        int h = t.height();
        int expected = static_cast<int>(std::ceil(1.44 * std::log2(101)));
        check("height O(log n)", h <= expected + 1);
        auto io = t.inorder();
        bool sorted = true;
        for (int i = 0; i < 99; ++i) sorted = sorted && io[i] < io[i+1];
        check("inorder sorted after 100 inserts", sorted);
    }

    {
        AvlTree t;
        for (int v : {5, 3, 7, 1, 4}) t.insert(v);
        check("remove leaf",   t.remove(1));
        check("remove middle", t.remove(3));
        auto io = t.inorder();
        check("after remove", io == std::vector<int>{4, 5, 7});
        check("remove absent", !t.remove(99));
    }

    {
        AvlTree t;
        try { t.min(); fail("min empty", "no exception"); }
        catch (const std::underflow_error&) { pass("min empty throws"); }
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
