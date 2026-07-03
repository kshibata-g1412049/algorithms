% test_union_find.m  (要素IDは0始まり)

uf = union_find('new', 5);
assert(union_find(uf, 'size') == 5);
assert(union_find(uf, 'components') == 5);
[~, b] = union_find(uf, 'connected', 0, 1);
assert(~b);

uf = union_find('new', 5);
[uf, ~] = union_find(uf, 'unite', 0, 1);
[uf, ~] = union_find(uf, 'unite', 1, 2);
[uf, b] = union_find(uf, 'connected', 0, 2);
assert(b);
[uf, b] = union_find(uf, 'connected', 0, 3);
assert(~b);
assert(union_find(uf, 'components') == 3);

uf = union_find('new', 5);
[uf, ~] = union_find(uf, 'unite', 0, 1);
[uf, ~] = union_find(uf, 'unite', 2, 3);
[uf, ~] = union_find(uf, 'unite', 0, 3);
[uf, b] = union_find(uf, 'connected', 1, 2);
assert(b);
assert(union_find(uf, 'components') == 2);
[uf, ok] = union_find(uf, 'unite', 0, 1);
assert(~ok);

disp('All tests passed.');
