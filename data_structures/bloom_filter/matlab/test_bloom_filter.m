% test_bloom_filter.m

bf = bloom_filter('new', 4096, 4);
for i = 0:99
    bf = bloom_filter(bf, 'insert', i);
end
all_found = true;
for i = 0:99
    if ~bloom_filter(bf, 'contains', i)
        all_found = false;
        break;
    end
end
assert(all_found);

bf2 = bloom_filter('new', 8, 3);
for i = 0:19
    bf2 = bloom_filter(bf2, 'insert', i);
end
fp = 0;
for i = 1000:1099
    if bloom_filter(bf2, 'contains', i), fp = fp + 1; end
end
assert(fp > 0);

disp('All tests passed.');
