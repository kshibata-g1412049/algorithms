% test_adjacency_list_graph.m  (頂点IDは0始まり)

g = adjacency_list_graph('new', 20);
g = adjacency_list_graph(g, 'add_vertex', 0);
g = adjacency_list_graph(g, 'add_vertex', 1);
g = adjacency_list_graph(g, 'add_vertex', 2);
assert(adjacency_list_graph(g, 'vertex_count') == 3);
assert(adjacency_list_graph(g, 'edge_count')   == 0);

g = adjacency_list_graph(g, 'add_edge', 0, 1);
g = adjacency_list_graph(g, 'add_edge', 0, 2);
g = adjacency_list_graph(g, 'add_edge', 1, 2);
assert(adjacency_list_graph(g, 'has_edge', 0, 1));
assert(adjacency_list_graph(g, 'has_edge', 0, 2));
assert(~adjacency_list_graph(g, 'has_edge', 2, 0));
assert(adjacency_list_graph(g, 'edge_count') == 3);

nb = adjacency_list_graph(g, 'get_neighbors', 0);
assert(any(nb == 1) && any(nb == 2));

try
    adjacency_list_graph(g, 'get_neighbors', 15);
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'adjacency_list_graph:noVertex'));
end

disp('All tests passed.');
