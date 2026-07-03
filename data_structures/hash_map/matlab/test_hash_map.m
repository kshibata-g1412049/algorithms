% test_hash_map.m

m = hash_map('new');
assert(hash_map(m, 'is_empty'));

m = hash_map(m, 'insert', 1, 10);
m = hash_map(m, 'insert', 2, 20);
m = hash_map(m, 'insert', 3, 30);
assert(hash_map(m, 'get', 1) == 10);
assert(hash_map(m, 'get', 3) == 30);
assert(hash_map(m, 'contains', 2));
assert(~hash_map(m, 'contains', 5));
assert(hash_map(m, 'size') == 3);

m = hash_map(m, 'insert', 1, 99);
assert(hash_map(m, 'get', 1) == 99);
assert(hash_map(m, 'size') == 3);

[m, ok] = hash_map(m, 'remove', 1);
assert(ok);
assert(~hash_map(m, 'contains', 1));
[m, ok] = hash_map(m, 'remove', 99);
assert(~ok);

m2 = hash_map('new');
try
    hash_map(m2, 'get', 99);
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'hash_map:keyNotFound'));
end

disp('All tests passed.');
