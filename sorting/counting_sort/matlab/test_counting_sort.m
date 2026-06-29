% test_counting_sort.m
%
% counting_sort関数のテストスクリプト（MATLAB/Octave両対応）。
% このフォルダを単独で取り出してそのまま実行できる（外部依存なし）。
% counting_sortは非負整数のみを対象とするため、負数のテストケースは含めない。
%
% 実行方法: MATLAB/Octave上でこのスクリプトを実行する。
%   >> test_counting_sort
% 全テストが成功すれば "All tests passed." が表示される。

assert(isequal(counting_sort([]), []));
assert(isequal(counting_sort([42]), [42]));
assert(isequal(counting_sort([1 2 3 4 5]), [1 2 3 4 5]));
assert(isequal(counting_sort([5 4 3 2 1]), [1 2 3 4 5]));
assert(isequal(counting_sort([3 1 2 3 1]), [1 1 2 3 3]));
assert(isequal(counting_sort([0 4 0 2 1 0]), [0 0 0 1 2 4]));

disp('All tests passed.');
