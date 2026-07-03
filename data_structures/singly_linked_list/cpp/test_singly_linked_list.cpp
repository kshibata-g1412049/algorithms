#include "singly_linked_list.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>
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
    { SinglyLinkedList l; check("new is empty", l.is_empty()); }

    {
        SinglyLinkedList l;
        l.push_front(3); l.push_front(2); l.push_front(1);
        auto v = l.to_vector();
        check("push_front order", v == std::vector<int>{1, 2, 3});
    }

    {
        SinglyLinkedList l;
        l.push_back(1); l.push_back(2); l.push_back(3);
        auto v = l.to_vector();
        check("push_back order", v == std::vector<int>{1, 2, 3});
        check("front", l.front() == 1);
        check("back", l.back() == 3);
    }

    {
        SinglyLinkedList l;
        l.push_back(1); l.push_back(2); l.push_back(3);
        check("pop_front", l.pop_front() == 1);
        check("front after pop", l.front() == 2);
        check("size after pop", l.size() == 2);
    }

    {
        SinglyLinkedList l;
        l.push_back(1); l.push_back(2); l.push_back(3);
        check("contains 2", l.contains(2));
        check("not contains 5", !l.contains(5));
    }

    {
        SinglyLinkedList l;
        l.push_back(1); l.push_back(2); l.push_back(3);
        check("remove middle", l.remove(2));
        auto v = l.to_vector();
        check("after remove", v == std::vector<int>{1, 3});
        check("remove absent", !l.remove(99));
    }

    {
        SinglyLinkedList l;
        try { l.front(); fail("front empty", "no exception"); }
        catch (const std::underflow_error&) { pass("front empty throws"); }
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
