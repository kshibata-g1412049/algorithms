#include "adjacency_matrix_graph.h"
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
        AdjacencyMatrixGraph g(4);
        check("vertex count 4", g.vertex_count() == 4);
        check("edge count 0",   g.edge_count() == 0);
    }

    {
        AdjacencyMatrixGraph g(4);
        g.add_edge(0, 1); g.add_edge(0, 2); g.add_edge(1, 3);
        check("has_edge 0-1",  g.has_edge(0, 1));
        check("has_edge 0-2",  g.has_edge(0, 2));
        check("no edge 1-0",   !g.has_edge(1, 0));
        check("edge count 3",  g.edge_count() == 3);
        auto nb = g.get_neighbors(0);
        check("neighbors 0",   nb == std::vector<int>{1, 2});
    }

    {
        AdjacencyMatrixGraph g(4);
        g.add_edge(0, 1); g.add_edge(0, 2);
        g.remove_edge(0, 1);
        check("after remove",  !g.has_edge(0, 1));
        check("edge count 1",  g.edge_count() == 1);
        g.add_edge(0, 1);
        check("no duplicate",  g.edge_count() == 2);
    }

    {
        AdjacencyMatrixGraph g(4);
        try { g.has_edge(0, 5); fail("bad vertex", "no exception"); }
        catch (const std::out_of_range&) { pass("bad vertex throws"); }
    }

    return allPassed ? EXIT_SUCCESS : EXIT_FAILURE;
}
