% test_adjacency_matrix_graph.m  (頂点IDは0始まり)

g = adjacency_matrix_graph('new', 4);
assert(adjacency_matrix_graph(g, 'vertex_count') == 4);
assert(adjacency_matrix_graph(g, 'edge_count')   == 0);

g = adjacency_matrix_graph(g, 'add_edge', 0, 1);
g = adjacency_matrix_graph(g, 'add_edge', 0, 2);
g = adjacency_matrix_graph(g, 'add_edge', 1, 3);
assert(adjacency_matrix_graph(g, 'has_edge', 0, 1));
assert(adjacency_matrix_graph(g, 'has_edge', 0, 2));
assert(~adjacency_matrix_graph(g, 'has_edge', 1, 0));
assert(adjacency_matrix_graph(g, 'edge_count') == 3);
nb = adjacency_matrix_graph(g, 'get_neighbors', 0);
assert(isequal(sort(nb), [1, 2]));

g = adjacency_matrix_graph(g, 'remove_edge', 0, 1);
assert(~adjacency_matrix_graph(g, 'has_edge', 0, 1));
assert(adjacency_matrix_graph(g, 'edge_count') == 2);

try
    adjacency_matrix_graph(g, 'has_edge', 0, 5);
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'adjacency_matrix_graph:outOfRange'));
end

disp('All tests passed.');
