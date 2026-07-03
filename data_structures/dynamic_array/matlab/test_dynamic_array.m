% test_dynamic_array.m — 動的配列のテストスクリプト

a = dynamic_array('new');
assert(dynamic_array(a, 'size') == 0);

a = dynamic_array(a, 'append', 10);
a = dynamic_array(a, 'append', 20);
a = dynamic_array(a, 'append', 30);
assert(dynamic_array(a, 'get', 1) == 10);
assert(dynamic_array(a, 'get', 2) == 20);
assert(dynamic_array(a, 'get', 3) == 30);
assert(dynamic_array(a, 'size') == 3);

a = dynamic_array(a, 'set', 2, 99);
assert(dynamic_array(a, 'get', 2) == 99);

a = dynamic_array(a, 'insert', 2, 55);
assert(dynamic_array(a, 'get', 1) == 10);
assert(dynamic_array(a, 'get', 2) == 55);
assert(dynamic_array(a, 'get', 3) == 99);
assert(dynamic_array(a, 'size') == 4);

a = dynamic_array(a, 'remove', 2);
assert(dynamic_array(a, 'get', 2) == 99);
assert(dynamic_array(a, 'size') == 3);

try
    dynamic_array(a, 'get', 99);
    error('expected error not thrown');
catch e
    assert(strcmp(e.identifier, 'dynamic_array:outOfRange'));
end

for i = 1:50
    a = dynamic_array(a, 'append', i);
end
assert(dynamic_array(a, 'size') == 53);
assert(dynamic_array(a, 'capacity') >= 53);

disp('All tests passed.');
