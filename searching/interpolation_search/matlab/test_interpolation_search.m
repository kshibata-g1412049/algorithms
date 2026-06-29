% test_interpolation_search.m
%
% interpolation_search関数のテストスクリプト（MATLAB/Octave両対応）。
% このフォルダを単独で取り出してそのまま実行できる（外部依存なし）。
%
% 実行方法: MATLAB/Octave上でこのスクリプトを実行する。
%   >> test_interpolation_search
% 全テストが成功すれば "All tests passed." が表示される。

% 空配列は常に-1
assert(isequal(interpolation_search([], 5), -1));

% 要素1つの配列：一致する場合
assert(isequal(interpolation_search([42], 42), 1));

% 要素1つの配列：一致しない場合
assert(isequal(interpolation_search([42], 7), -1));

% targetが先頭にある場合
arr = [10 20 30 40 50 60 70 80 90];
assert(isequal(interpolation_search(arr, 10), 1));

% targetが末尾にある場合
assert(isequal(interpolation_search(arr, 90), 9));

% targetが中央にある場合
assert(isequal(interpolation_search(arr, 50), 5));

% targetが存在しない場合（最小値より小さい/最大値より大きい/間に挟まれる）
assert(isequal(interpolation_search(arr, 5), -1));
assert(isequal(interpolation_search(arr, 100), -1));
assert(isequal(interpolation_search(arr, 45), -1));

% 重複要素を含む配列：複数の有効な解があり得るので、返ったインデックスの
% 値がtargetと一致するかを確認する
arr2 = [2 4 4 4 6 8 10];
idx = interpolation_search(arr2, 4);
assert(idx >= 1 && idx <= length(arr2));
assert(arr2(idx) == 4);

% 全要素が同じ値の配列（分母が0になるケースの確認）
arr3 = [5 5 5 5 5];
idx2 = interpolation_search(arr3, 5);
assert(idx2 >= 1 && idx2 <= length(arr3));
assert(arr3(idx2) == 5);
assert(isequal(interpolation_search(arr3, 9), -1));

disp('All tests passed.');
