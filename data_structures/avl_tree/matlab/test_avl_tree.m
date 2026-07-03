% test_avl_tree.m

t = avl_tree('new');
assert(avl_tree(t, 'is_empty'));

for v = [5, 3, 7, 1, 4]
    t = avl_tree(t, 'insert', v);
end
assert(avl_tree(t, 'search', 3));
assert(~avl_tree(t, 'search', 6));
assert(isequal(avl_tree(t, 'inorder'), [1, 3, 4, 5, 7]));

t2 = avl_tree('new');
for v = [5, 3, 7]
    t2 = avl_tree(t2, 'insert', v);
end
assert(avl_tree(t2, 'min') == 3);
assert(avl_tree(t2, 'max') == 7);

t3 = avl_tree('new');
for i = 1:100
    t3 = avl_tree(t3, 'insert', i);
end
h = avl_tree(t3, 'height');
assert(h <= ceil(1.44 * log2(101)) + 1);
io = avl_tree(t3, 'inorder');
assert(isequal(io, 1:100));

[t2, ok] = avl_tree(t2, 'remove', 3);
assert(ok);
assert(isequal(avl_tree(t2, 'inorder'), [5, 7]));
[t2, ok] = avl_tree(t2, 'remove', 99);
assert(~ok);

t_empty = avl_tree('new');
try
    avl_tree(t_empty, 'min');
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'avl_tree:underflow'));
end

disp('All tests passed.');
