% test_singly_linked_list.m

ll = singly_linked_list('new');
assert(singly_linked_list(ll, 'is_empty'));

ll = singly_linked_list(ll, 'push_front', 3);
ll = singly_linked_list(ll, 'push_front', 2);
ll = singly_linked_list(ll, 'push_front', 1);
assert(isequal(singly_linked_list(ll, 'to_array'), [1, 2, 3]));

ll = singly_linked_list('new');
ll = singly_linked_list(ll, 'push_back', 1);
ll = singly_linked_list(ll, 'push_back', 2);
ll = singly_linked_list(ll, 'push_back', 3);
assert(isequal(singly_linked_list(ll, 'to_array'), [1, 2, 3]));
assert(singly_linked_list(ll, 'front') == 1);
assert(singly_linked_list(ll, 'back')  == 3);

[ll, v] = singly_linked_list(ll, 'pop_front');
assert(v == 1);
assert(singly_linked_list(ll, 'front') == 2);
assert(singly_linked_list(ll, 'size')  == 2);

ll = singly_linked_list('new');
ll = singly_linked_list(ll, 'push_back', 1);
ll = singly_linked_list(ll, 'push_back', 2);
ll = singly_linked_list(ll, 'push_back', 3);
assert(singly_linked_list(ll, 'contains', 2));
assert(~singly_linked_list(ll, 'contains', 5));

[ll, ok] = singly_linked_list(ll, 'remove', 2);
assert(ok);
assert(isequal(singly_linked_list(ll, 'to_array'), [1, 3]));
[ll, ok] = singly_linked_list(ll, 'remove', 99);
assert(~ok);

ll_empty = singly_linked_list('new');
try
    singly_linked_list(ll_empty, 'front');
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'singly_linked_list:underflow'));
end

disp('All tests passed.');
