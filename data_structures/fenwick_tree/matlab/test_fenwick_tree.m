% test_fenwick_tree.m  (1始まりインデックス)

ft = fenwick_tree('new', 6);
for i = 1:6
    ft = fenwick_tree(ft, 'update', i, i);
end
assert(fenwick_tree(ft, 'prefix_sum', 6) == 21);
assert(fenwick_tree(ft, 'range_sum', 2, 4) == 9);

data = [1, 3, 5, 7, 9, 11];
ft2 = fenwick_tree('from_data', data);
assert(fenwick_tree(ft2, 'prefix_sum', 6) == 36);
assert(fenwick_tree(ft2, 'range_sum', 2, 4) == 15);
ft2 = fenwick_tree(ft2, 'update', 3, 5);
assert(fenwick_tree(ft2, 'range_sum', 2, 4) == 20);

data3 = 1:100;
ft3 = fenwick_tree('from_data', data3);
assert(fenwick_tree(ft3, 'range_sum', 21, 50) == sum(data3(21:50)));
ft3 = fenwick_tree(ft3, 'update', 31, 900);
data3(31) = data3(31) + 900;
assert(fenwick_tree(ft3, 'range_sum', 21, 50) == sum(data3(21:50)));

try
    fenwick_tree(ft, 'update', 99, 0);
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'fenwick_tree:outOfRange'));
end

disp('All tests passed.');
