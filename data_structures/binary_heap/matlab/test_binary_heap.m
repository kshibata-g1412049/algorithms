% test_binary_heap.m

h = binary_heap('new');
assert(binary_heap(h, 'is_empty'));

for v = [5, 3, 7, 1]
    h = binary_heap(h, 'insert', v);
end
assert(binary_heap(h, 'find_min') == 1);
assert(binary_heap(h, 'size') == 4);

h2 = binary_heap('new');
for v = [5, 3, 7, 1, 4, 2, 6]
    h2 = binary_heap(h2, 'insert', v);
end
out = zeros(1, 7);
for i = 1:7
    [h2, out(i)] = binary_heap(h2, 'extract_min');
end
assert(isequal(out, [1, 2, 3, 4, 5, 6, 7]));

h3 = binary_heap('new');
for i = 100:-1:1
    h3 = binary_heap(h3, 'insert', i);
end
[h3, prev] = binary_heap(h3, 'extract_min');
while ~binary_heap(h3, 'is_empty')
    [h3, cur] = binary_heap(h3, 'extract_min');
    assert(cur >= prev);
    prev = cur;
end

h_empty = binary_heap('new');
try
    binary_heap(h_empty, 'find_min');
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'binary_heap:underflow'));
end

disp('All tests passed.');
