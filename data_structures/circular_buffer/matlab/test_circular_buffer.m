% test_circular_buffer.m — 循環バッファのテストスクリプト

b = circular_buffer('new', 3);
assert(circular_buffer(b, 'is_empty'));
assert(circular_buffer(b, 'capacity') == 3);

b = circular_buffer(b, 'push', 10);
b = circular_buffer(b, 'push', 20);
b = circular_buffer(b, 'push', 30);
assert(circular_buffer(b, 'is_full'));
assert(circular_buffer(b, 'peek') == 10);

[b, v] = circular_buffer(b, 'pop');
assert(v == 10);
[b, v] = circular_buffer(b, 'pop');
assert(v == 20);

b = circular_buffer(b, 'push', 40);
b = circular_buffer(b, 'push', 50);
assert(circular_buffer(b, 'is_full'));
assert(circular_buffer(b, 'peek') == 30);

try
    circular_buffer(b, 'push', 99);
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'circular_buffer:overflow'));
end

[b, ~] = circular_buffer(b, 'pop');
[b, ~] = circular_buffer(b, 'pop');
[b, ~] = circular_buffer(b, 'pop');
assert(circular_buffer(b, 'is_empty'));

try
    circular_buffer(b, 'pop');
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'circular_buffer:underflow'));
end

disp('All tests passed.');
