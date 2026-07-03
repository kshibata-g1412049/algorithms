% test_stack.m — スタックのテストスクリプト

s = stack('new');
assert(stack(s, 'is_empty'));
assert(stack(s, 'size') == 0);

s = stack(s, 'push', 10);
assert(stack(s, 'peek') == 10);
assert(stack(s, 'size') == 1);
assert(~stack(s, 'is_empty'));

s = stack(s, 'push', 20);
s = stack(s, 'push', 30);
[s, v] = stack(s, 'pop');
assert(v == 30);
[s, v] = stack(s, 'pop');
assert(v == 20);
assert(stack(s, 'peek') == 10);
assert(stack(s, 'size') == 1);

[s, ~] = stack(s, 'pop');
assert(stack(s, 'is_empty'));

for i = 1:50
    s = stack(s, 'push', i);
end
assert(stack(s, 'size') == 50);
assert(stack(s, 'peek') == 50);

try
    stack(s, 'pop');  % まだ要素あり — 正常
    empty_s = stack('new');
    stack(empty_s, 'pop');  % 空なのでエラー
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'stack:underflow'));
end

disp('All tests passed.');
