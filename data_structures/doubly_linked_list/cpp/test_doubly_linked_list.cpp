#include "doubly_linked_list.h"
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
    { DoublyLinkedList l; check("new is empty", l.is_empty()); }

    {
        DoublyLinkedList l;
        l.push_front(3); l.push_front(2); l.push_front(1);
        check("push_front order", l.to_vector() == std::vector<int>{1, 2, 3});
    }

    {
        DoublyLinkedList l;
        l.push_back(1); l.push_back(2); l.push_back(3);
        check("push_back order", l.to_vector() == std::vector<int>{1, 2, 3});
        check("front", l.front() == 1);
        check("back",  l.back()  == 3);
    }

    {
        DoublyLinkedList l;
        l.push_back(1); l.push_back(2); l.push_back(3);
        check("pop_front", l.pop_front() == 1);
        check("front after pop_front", l.front() == 2);
        check("pop_back",  l.pop_back()  == 3);
        check("back after pop_back",   l.back()  == 2);
        check("size after pops", l.size() == 1);
    }

    {
        DoublyLinkedList l;
        l.push_back(1); l.push_back(2); l.push_back(3);
        check("contains 2",    l.contains(2));
        check("not contains 5", !l.contains(5));
    }

    {
        DoublyLinkedList l;
        l.push_back(1); l.push_back(2); l.push_back(3);
        check("remove middle", l.remove(2));
        check("after remove",  l.to_vector() == std::vector<int>{1, 3});
        check("remove head",   l.remove(1));
        check("after head rm", l.to_vector() == std::vector<int>{3});
        check("remove absent", !l.remove(99));
    }

    {
        DoublyLinkedList l;
        try { l.front(); fail("front empty", "no exception"); }
        catch (const std::underflow_error&) { pass("front empty throws"); }
        try { l.pop_back(); fail("pop_back empty", "no exception"); }
        catch (const std::underflow_error&) { pass("pop_back empty throws"); }
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
