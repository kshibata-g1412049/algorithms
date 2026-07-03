% test_hash_set.m

s = hash_set('new');
assert(hash_set(s, 'is_empty'));

s = hash_set(s, 'insert', 1);
s = hash_set(s, 'insert', 2);
s = hash_set(s, 'insert', 3);
assert(hash_set(s, 'contains', 1));
assert(~hash_set(s, 'contains', 5));
assert(hash_set(s, 'size') == 3);

s = hash_set(s, 'insert', 1);
assert(hash_set(s, 'size') == 3);

[s, ok] = hash_set(s, 'remove', 2);
assert(ok);
assert(~hash_set(s, 'contains', 2));
assert(hash_set(s, 'contains', 1));
[s, ok] = hash_set(s, 'remove', 99);
assert(~ok);
assert(hash_set(s, 'size') == 2);

disp('All tests passed.');
