% test_linear_search.m
%
% linear_search関数のテストスクリプト（MATLAB/Octave両対応）。
% このフォルダを単独で取り出してそのまま実行できる（外部依存なし）。
%
% 実行方法: MATLAB/Octave上でこのスクリプトを実行する。
%   >> test_linear_search
% 全テストが成功すれば "All tests passed." が表示される。

% 空配列は常に-1
assert(isequal(linear_search([], 5), -1));

% 要素1つの配列：一致する場合
assert(isequal(linear_search([42], 42), 1));

% 要素1つの配列：一致しない場合
assert(isequal(linear_search([42], 7), -1));

% targetが先頭にある場合
assert(isequal(linear_search([1 3 5 7 9], 1), 1));

% targetが末尾にある場合
assert(isequal(linear_search([1 3 5 7 9], 9), 5));

% targetが中央にある場合
assert(isequal(linear_search([1 3 5 7 9], 5), 3));

% targetが存在しない場合（最小値より小さい/最大値より大きい/間に挟まれる）
arr = [10 20 30 40 50];
assert(isequal(linear_search(arr, 5), -1));
assert(isequal(linear_search(arr, 100), -1));
assert(isequal(linear_search(arr, 25), -1));

% 重複要素を含む配列：複数の有効な解があり得るので、返ったインデックスの
% 値がtargetと一致するかを確認する
arr2 = [2 4 4 4 6];
idx = linear_search(arr2, 4);
assert(idx >= 1 && idx <= length(arr2));
assert(arr2(idx) == 4);

disp('All tests passed.');
