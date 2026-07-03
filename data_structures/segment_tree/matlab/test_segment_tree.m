% test_segment_tree.m  (1始まりインデックス)

data = [1, 3, 5, 7, 9, 11];
st = segment_tree('new', data);
assert(segment_tree(st, 'size') == 6);
assert(segment_tree(st, 'query', 1, 6) == 36);
assert(segment_tree(st, 'query', 2, 4) == 15);
assert(segment_tree(st, 'query', 3, 3) == 5);

st = segment_tree(st, 'update', 3, 10);
assert(segment_tree(st, 'query', 1, 6) == 41);
assert(segment_tree(st, 'query', 2, 4) == 20);

data2 = 1:100;
st2 = segment_tree('new', data2);
assert(segment_tree(st2, 'query', 21, 50) == sum(data2(21:50)));
st2 = segment_tree(st2, 'update', 31, 999);
data2(31) = 999;
assert(segment_tree(st2, 'query', 21, 50) == sum(data2(21:50)));

try
    segment_tree(st, 'update', 99, 0);
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'segment_tree:outOfRange'));
end

disp('All tests passed.');
