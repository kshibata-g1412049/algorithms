% test_deque.m — 両端キューのテストスクリプト

d = deque('new');
assert(deque(d, 'is_empty'));

d = deque(d, 'push_back', 1);
d = deque(d, 'push_back', 2);
d = deque(d, 'push_back', 3);
assert(deque(d, 'peek_front') == 1);
assert(deque(d, 'peek_back') == 3);
assert(deque(d, 'size') == 3);

[d, v] = deque(d, 'pop_front');
assert(v == 1);
[d, v] = deque(d, 'pop_back');
assert(v == 3);
assert(deque(d, 'size') == 1);

d = deque(d, 'push_front', 0);
assert(deque(d, 'peek_front') == 0);
assert(deque(d, 'peek_back') == 2);

[d, ~] = deque(d, 'pop_front');
[d, ~] = deque(d, 'pop_front');
assert(deque(d, 'is_empty'));

try
    empty_d = deque('new');
    deque(empty_d, 'pop_front');
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'deque:underflow'));
end

disp('All tests passed.');
