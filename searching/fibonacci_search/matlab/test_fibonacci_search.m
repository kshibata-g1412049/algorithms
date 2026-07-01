% test_fibonacci_search.m
%
% fibonacci_search関数のテストスクリプト（MATLAB/Octave両対応）。
% このフォルダを単独で取り出してそのまま実行できる（外部依存なし）。
%
% 実行方法: MATLAB/Octave上でこのスクリプトを実行する。
%   >> test_fibonacci_search
% 全テストが成功すれば "All tests passed." が表示される。

% 空配列は常に-1
assert(isequal(fibonacci_search([], 5), -1));

% 要素1つの配列：一致する場合
assert(isequal(fibonacci_search([42], 42), 1));

% 要素1つの配列：一致しない場合
assert(isequal(fibonacci_search([42], 7), -1));

% targetが先頭にある場合
arr = [1 3 5 7 9 11 13 15 17];
assert(isequal(fibonacci_search(arr, 1), 1));

% targetが末尾にある場合
assert(isequal(fibonacci_search(arr, 17), 9));

% targetが中央にある場合
assert(isequal(fibonacci_search(arr, 9), 5));

% targetが存在しない場合（最小値より小さい/最大値より大きい/間に挟まれる）
arr2 = [10 20 30 40 50 60 70 80 90];
assert(isequal(fibonacci_search(arr2, 5), -1));
assert(isequal(fibonacci_search(arr2, 100), -1));
assert(isequal(fibonacci_search(arr2, 45), -1));

% 重複要素を含む配列：複数の有効な解があり得るので、返ったインデックスの
% 値がtargetと一致するかを確認する
arr3 = [2 4 4 4 6 8 10];
idx = fibonacci_search(arr3, 4);
assert(idx >= 1 && idx <= length(arr3));
assert(arr3(idx) == 4);

% 2要素の配列（境界値の確認）
arr4 = [3 8];
assert(isequal(fibonacci_search(arr4, 3), 1));
assert(isequal(fibonacci_search(arr4, 8), 2));

disp('All tests passed.');
