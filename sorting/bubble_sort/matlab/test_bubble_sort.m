% test_bubble_sort.m
%
% bubble_sort関数のテストスクリプト（MATLAB/Octave両対応）。
% このフォルダを単独で取り出してそのまま実行できる（外部依存なし）。
%
% 実行方法: MATLAB/Octave上でこのスクリプトを実行する。
%   >> test_bubble_sort
% 全テストが成功すれば "All tests passed." が表示される。

% 空配列
assert(isequal(bubble_sort([]), []));

% 要素1つの配列
assert(isequal(bubble_sort([42]), [42]));

% 既にソート済みの配列
assert(isequal(bubble_sort([1 2 3 4 5]), [1 2 3 4 5]));

% 完全な逆順の配列
assert(isequal(bubble_sort([5 4 3 2 1]), [1 2 3 4 5]));

% 重複要素を含む配列
assert(isequal(bubble_sort([3 1 2 3 1]), [1 1 2 3 3]));

% 負数を含む配列
assert(isequal(bubble_sort([-3 5 -1 0 2 -8]), [-8 -3 -1 0 2 5]));

disp('All tests passed.');
