% test_trie.m

t = trie('new');
assert(trie(t, 'size') == 0);

t = trie(t, 'insert', 'apple');
assert(trie(t, 'search', 'apple'));
assert(~trie(t, 'search', 'app'));
assert(trie(t, 'starts_with', 'app'));
assert(~trie(t, 'starts_with', 'ban'));
assert(trie(t, 'size') == 1);

t = trie(t, 'insert', 'app');
assert(trie(t, 'search', 'app'));
assert(trie(t, 'search', 'apple'));
assert(trie(t, 'size') == 2);
[t, ok] = trie(t, 'remove', 'app');
assert(ok);
assert(trie(t, 'search', 'apple'));
assert(~trie(t, 'search', 'app'));
assert(trie(t, 'size') == 1);

t2 = trie('new');
t2 = trie(t2, 'insert', 'cat');
t2 = trie(t2, 'insert', 'car');
t2 = trie(t2, 'insert', 'card');
assert(trie(t2, 'starts_with', 'ca'));
assert(trie(t2, 'starts_with', 'car'));
assert(~trie(t2, 'starts_with', 'cb'));
[~, ok] = trie(t2, 'remove', 'cap');
assert(~ok);

disp('All tests passed.');
