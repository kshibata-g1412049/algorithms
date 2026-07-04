#include "segment_tree.h"
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
        std::vector<long long> data = {1, 3, 5, 7, 9, 11};
        SegmentTree<long long> st(data);
        check("size", st.size() == 6);
        check("full sum",    st.query(0, 6) == 36);
        check("partial sum", st.query(1, 4) == 15);
        check("single",      st.query(2, 3) == 5);
    }

    {
        std::vector<long long> data = {1, 3, 5, 7, 9, 11};
        SegmentTree<long long> st(data);
        st.update(2, 10);
        check("after update full",    st.query(0, 6) == 41);
        check("after update partial", st.query(1, 4) == 20);
    }

    {
        // Brute-force verification
        std::vector<long long> data(100);
        std::iota(data.begin(), data.end(), 1);
        SegmentTree<long long> st(data);
        long long expected = 0;
        for (int i = 20; i < 50; ++i) expected += data[i];
        check("brute-force match", st.query(20, 50) == expected);
        data[30] = 999;
        st.update(30, 999);
        expected = 0;
        for (int i = 20; i < 50; ++i) expected += data[i];
        check("brute-force after update", st.query(20, 50) == expected);
    }

    {
        std::vector<long long> data = {1, 2, 3};
        SegmentTree<long long> st(data);
        try { st.update(5, 0); fail("bad update index", "no exception"); }
        catch (const std::out_of_range&) { pass("bad update throws"); }
        try { st.query(2, 1); fail("bad query range", "no exception"); }
        catch (const std::out_of_range&) { pass("bad query throws"); }
    }

    // --- 非整数型（テンプレート動作確認） ---
    {
        std::vector<double> data = {0.5, 1.5, 2.0};
        SegmentTree<double> st(data);
        check("double full sum", st.query(0, 3) == 4.0);
        st.update(1, 2.5);
        check("double after update", st.query(0, 3) == 5.0);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
