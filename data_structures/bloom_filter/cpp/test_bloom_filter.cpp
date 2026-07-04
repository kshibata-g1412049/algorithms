#include "bloom_filter.h"
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
    {
        BloomFilter<int> bf(1024, 3);
        bf.insert(1); bf.insert(2); bf.insert(3);
        check("contains 1", bf.contains(1));
        check("contains 2", bf.contains(2));
        check("contains 3", bf.contains(3));
    }

    {
        // No false negatives: every inserted element must be found
        BloomFilter<int> bf(4096, 4);
        for (int i = 0; i < 100; ++i) bf.insert(i);
        bool no_false_neg = true;
        for (int i = 0; i < 100; ++i)
            if (!bf.contains(i)) { no_false_neg = false; break; }
        check("no false negatives", no_false_neg);
    }

    {
        // Very small filter should have false positives (design behaviour)
        BloomFilter<int> bf(8, 3);
        for (int i = 0; i < 20; ++i) bf.insert(i);
        int fp = 0;
        for (int i = 1000; i < 1100; ++i)
            if (bf.contains(i)) ++fp;
        check("false positives exist in tiny filter", fp > 0);
    }

    // --- 非int型（テンプレート動作確認） ---
    {
        BloomFilter<std::string> bf(1024, 3);
        bf.insert("hello"); bf.insert("world");
        check("string no false negative", bf.contains("hello") && bf.contains("world"));
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
