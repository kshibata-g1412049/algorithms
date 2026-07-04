#include "binary_heap.h"
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
    { BinaryHeap<int> h; check("new is empty", h.is_empty()); }

    {
        BinaryHeap<int> h;
        h.insert(5); h.insert(3); h.insert(7); h.insert(1);
        check("find_min", h.find_min() == 1);
        check("size 4",   h.size() == 4);
    }

    {
        BinaryHeap<int> h;
        for (int v : {5, 3, 7, 1, 4, 2, 6}) h.insert(v);
        std::vector<int> out;
        while (!h.is_empty()) out.push_back(h.extract_min());
        check("extract_min order", out == std::vector<int>{1, 2, 3, 4, 5, 6, 7});
    }

    {
        BinaryHeap<int> h;
        for (int i = 100; i >= 1; --i) h.insert(i);
        bool sorted = true;
        int prev = h.extract_min();
        while (!h.is_empty()) {
            int cur = h.extract_min();
            if (cur < prev) { sorted = false; break; }
            prev = cur;
        }
        check("100 elements sorted", sorted);
    }

    {
        BinaryHeap<int> h;
        try { h.find_min(); fail("find_min empty", "no exception"); }
        catch (const std::underflow_error&) { pass("find_min empty throws"); }
        try { h.extract_min(); fail("extract_min empty", "no exception"); }
        catch (const std::underflow_error&) { pass("extract_min empty throws"); }
    }

    // --- 非int型・Compare（テンプレート動作確認） ---
    {
        BinaryHeap<std::string> h;
        h.insert("pear"); h.insert("apple"); h.insert("mango");
        check("string min", h.extract_min() == "apple");
    }
    {
        // std::greater で最大ヒープとして使う
        BinaryHeap<int, std::greater<int>> h;
        h.insert(3); h.insert(10); h.insert(5);
        check("max-heap extract", h.extract_min() == 10);
        check("max-heap second", h.extract_min() == 5);
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
