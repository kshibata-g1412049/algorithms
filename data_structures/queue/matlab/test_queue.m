% test_queue.m — キューのテストスクリプト

q = queue('new');
assert(queue(q, 'is_empty'));
assert(queue(q, 'size') == 0);

q = queue(q, 'enqueue', 10);
q = queue(q, 'enqueue', 20);
q = queue(q, 'enqueue', 30);
assert(queue(q, 'peek') == 10);
assert(queue(q, 'size') == 3);

[q, v] = queue(q, 'dequeue');
assert(v == 10);
[q, v] = queue(q, 'dequeue');
assert(v == 20);
assert(queue(q, 'peek') == 30);
[q, v] = queue(q, 'dequeue');
assert(v == 30);
assert(queue(q, 'is_empty'));

try
    empty_q = queue('new');
    queue(empty_q, 'dequeue');
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'queue:underflow'));
end

for i = 1:50
    q = queue(q, 'enqueue', i);
end
assert(queue(q, 'size') == 50);
assert(queue(q, 'peek') == 1);

disp('All tests passed.');
