% test_binary_search_tree.m

t = binary_search_tree('new');
assert(binary_search_tree(t, 'is_empty'));

for v = [5, 3, 7, 1, 4]
    t = binary_search_tree(t, 'insert', v);
end
assert(binary_search_tree(t, 'search', 3));
assert(binary_search_tree(t, 'search', 7));
assert(~binary_search_tree(t, 'search', 6));
assert(binary_search_tree(t, 'size') == 5);

io = binary_search_tree(t, 'inorder');
assert(isequal(io, [1, 3, 4, 5, 7]));

t2 = binary_search_tree('new');
for v = [5, 3, 7]
    t2 = binary_search_tree(t2, 'insert', v);
end
assert(binary_search_tree(t2, 'min') == 3);
assert(binary_search_tree(t2, 'max') == 7);

[t2, ok] = binary_search_tree(t2, 'remove', 3);
assert(ok);
assert(isequal(binary_search_tree(t2, 'inorder'), [5, 7]));
[t2, ok] = binary_search_tree(t2, 'remove', 99);
assert(~ok);

t3 = binary_search_tree('new');
t3 = binary_search_tree(t3, 'insert', 5);
t3 = binary_search_tree(t3, 'insert', 5);
assert(binary_search_tree(t3, 'size') == 1);

t_empty = binary_search_tree('new');
try
    binary_search_tree(t_empty, 'min');
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'binary_search_tree:underflow'));
end

disp('All tests passed.');
