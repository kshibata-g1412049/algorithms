% test_doubly_linked_list.m

ll = doubly_linked_list('new');
assert(doubly_linked_list(ll, 'is_empty'));

ll = doubly_linked_list(ll, 'push_front', 3);
ll = doubly_linked_list(ll, 'push_front', 2);
ll = doubly_linked_list(ll, 'push_front', 1);
assert(isequal(doubly_linked_list(ll, 'to_array'), [1, 2, 3]));

ll = doubly_linked_list('new');
ll = doubly_linked_list(ll, 'push_back', 1);
ll = doubly_linked_list(ll, 'push_back', 2);
ll = doubly_linked_list(ll, 'push_back', 3);
assert(isequal(doubly_linked_list(ll, 'to_array'), [1, 2, 3]));
assert(doubly_linked_list(ll, 'front') == 1);
assert(doubly_linked_list(ll, 'back')  == 3);

[ll, v] = doubly_linked_list(ll, 'pop_front');
assert(v == 1);
[ll, v] = doubly_linked_list(ll, 'pop_back');
assert(v == 3);
assert(doubly_linked_list(ll, 'size') == 1);

ll = doubly_linked_list('new');
ll = doubly_linked_list(ll, 'push_back', 1);
ll = doubly_linked_list(ll, 'push_back', 2);
ll = doubly_linked_list(ll, 'push_back', 3);
assert(doubly_linked_list(ll, 'contains', 2));
assert(~doubly_linked_list(ll, 'contains', 5));

[ll, ok] = doubly_linked_list(ll, 'remove', 2);
assert(ok);
assert(isequal(doubly_linked_list(ll, 'to_array'), [1, 3]));
[ll, ok] = doubly_linked_list(ll, 'remove', 99);
assert(~ok);

ll_empty = doubly_linked_list('new');
try
    doubly_linked_list(ll_empty, 'front');
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'doubly_linked_list:underflow'));
end

disp('All tests passed.');
