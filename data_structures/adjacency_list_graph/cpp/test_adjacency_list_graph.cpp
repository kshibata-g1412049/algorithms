#include "adjacency_list_graph.h"
#include <algorithm>
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
    {
        AdjacencyListGraph g;
        g.add_vertex(0); g.add_vertex(1); g.add_vertex(2);
        check("vertex count 3", g.vertex_count() == 3);
        check("edge count 0",   g.edge_count() == 0);
    }

    {
        AdjacencyListGraph g;
        g.add_edge(0, 1); g.add_edge(0, 2); g.add_edge(1, 2);
        check("has_edge 0-1",    g.has_edge(0, 1));
        check("has_edge 0-2",    g.has_edge(0, 2));
        check("no edge 2-0",     !g.has_edge(2, 0));
        check("edge count 3",    g.edge_count() == 3);
        check("vertex count 3",  g.vertex_count() == 3);
        auto nb = g.get_neighbors(0);
        check("neighbors 0",
            std::find(nb.begin(), nb.end(), 1) != nb.end() &&
            std::find(nb.begin(), nb.end(), 2) != nb.end());
    }

    {
        AdjacencyListGraph g;
        try { g.get_neighbors(99); fail("bad vertex", "no exception"); }
        catch (const std::out_of_range&) { pass("bad vertex throws"); }
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
