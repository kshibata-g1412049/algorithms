#include "fenwick_tree.h"
#include <cstdlib>
#include <iostream>
#include <numeric>
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
    {
        FenwickTree<long long> ft(6);
        for (int i = 0; i < 6; ++i) ft.update(i, i + 1);
        check("prefix 6",   ft.prefix_sum(6) == 21);
        check("range 1-4",  ft.range_sum(1, 4) == 9);
        check("range 0-3",  ft.range_sum(0, 3) == 6);
    }

    {
        std::vector<long long> data = {1, 3, 5, 7, 9, 11};
        FenwickTree<long long> ft(data);
        check("build prefix", ft.prefix_sum(6) == 36);
        check("build range",  ft.range_sum(1, 4) == 15);
        ft.update(2, 5);
        check("after update", ft.range_sum(1, 4) == 20);
    }

    {
        std::vector<long long> data(100);
        std::iota(data.begin(), data.end(), 1);
        FenwickTree<long long> ft(data);
        long long expected = 0;
        for (int i = 20; i < 50; ++i) expected += data[i];
        check("brute-force", ft.range_sum(20, 50) == expected);
        ft.update(30, 900);
        expected += 900;
        check("after update brute", ft.range_sum(20, 50) == expected);
    }

    {
        FenwickTree<long long> ft(3);
        try { ft.update(5, 1); fail("bad update", "no exception"); }
        catch (const std::out_of_range&) { pass("bad update throws"); }
        try { ft.range_sum(2, 1); fail("bad range", "no exception"); }
        catch (const std::out_of_range&) { pass("bad range throws"); }
    }

    // --- 非整数型（テンプレート動作確認） ---
    {
        FenwickTree<double> ft(3);
        ft.update(0, 0.5); ft.update(1, 1.5); ft.update(2, 2.0);
        check("double prefix", ft.prefix_sum(3) == 4.0);
        check("double range", ft.range_sum(1, 3) == 3.5);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
