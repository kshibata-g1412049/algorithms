#include "union_find.h"
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
    {
        UnionFind uf(5);
        check("size 5",         uf.size() == 5);
        check("components 5",   uf.components() == 5);
        check("not connected",  !uf.connected(0, 1));
    }

    {
        UnionFind uf(5);
        uf.unite(0, 1); uf.unite(1, 2);
        check("connected 0-2",  uf.connected(0, 2));
        check("not 0-3",        !uf.connected(0, 3));
        check("components 3",   uf.components() == 3);
    }

    {
        UnionFind uf(5);
        uf.unite(0, 1); uf.unite(2, 3); uf.unite(0, 3);
        check("transitivity",   uf.connected(1, 2));
        check("components 2",   uf.components() == 2);
        check("dup unite false", !uf.unite(0, 1));
    }

    {
        UnionFind uf(3);
        try { uf.find(5); fail("bad element", "no exception"); }
        catch (const std::out_of_range&) { pass("bad element throws"); }
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
