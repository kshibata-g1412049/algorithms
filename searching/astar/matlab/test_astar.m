% test_astar.m
%
% astar関数のテストスクリプト（MATLAB/Octave両対応）。
% このフォルダを単独で取り出してそのまま実行できる（外部依存なし）。
% ヘルパー関数(pathCost/isValidPath)はscript内のローカル関数として
% 使うとMATLAB/Octaveで定義順序の制約があるため、各テストケース内に
% 検証ロジックを直接書く方式にしている。
%
% 実行方法: MATLAB/Octave上でこのスクリプトを実行する。
%   >> test_astar
% 全テストが成功すれば "All tests passed." が表示される。

sqrt2 = sqrt(2);
eps_tol = 1e-6;

% --- 障害物なしの直線（斜め）経路。最短コストは4*sqrt(2) ---
grid1 = zeros(5, 5);
startPos = [1 1];
goalPos = [5 5];
path1 = astar(grid1, startPos, goalPos);
assert(~isempty(path1));
assert(isequal(path1(1, :), startPos) && isequal(path1(end, :), goalPos));
cost1 = 0;
for i = 2:size(path1, 1)
    dr = abs(path1(i, 1) - path1(i - 1, 1));
    dc = abs(path1(i, 2) - path1(i - 1, 2));
    assert(dr <= 1 && dc <= 1 && ~(dr == 0 && dc == 0));
    if dr == 1 && dc == 1
        cost1 = cost1 + sqrt2;
    else
        cost1 = cost1 + 1;
    end
end
assert(abs(cost1 - 4 * sqrt2) < eps_tol);

% --- 壁に迂回が必要なケース。最短コストは8.0 ---
grid2 = [
    0 0 0 0 0;
    0 1 1 1 0;
    0 0 0 0 0;
    0 1 1 1 0;
    0 0 0 0 0
];
startPos = [1 1];
goalPos = [5 5];
path2 = astar(grid2, startPos, goalPos);
assert(~isempty(path2));
assert(isequal(path2(1, :), startPos) && isequal(path2(end, :), goalPos));
cost2 = 0;
for i = 2:size(path2, 1)
    r = path2(i, 1);
    c = path2(i, 2);
    assert(grid2(r, c) == 0); % 障害物を通らない
    dr = path2(i, 1) - path2(i - 1, 1);
    dc = path2(i, 2) - path2(i - 1, 2);
    assert(abs(dr) <= 1 && abs(dc) <= 1 && ~(dr == 0 && dc == 0));
    if dr ~= 0 && dc ~= 0
        r0 = path2(i - 1, 1);
        c0 = path2(i - 1, 2);
        assert(grid2(r0 + dr, c0) == 0 && grid2(r0, c0 + dc) == 0); % コーナーカット禁止
        cost2 = cost2 + sqrt2;
    else
        cost2 = cost2 + 1;
    end
end
assert(abs(cost2 - 8.0) < eps_tol);

% --- startとgoalが同一の場合 ---
grid3 = zeros(3, 3);
path3 = astar(grid3, [2 2], [2 2]);
assert(isequal(path3, [2 2]));

% --- 到達不可能なケース（完全に壁で塞がれている） ---
grid4 = [
    0 0 0 0 0;
    1 1 1 1 1;
    0 0 0 0 0;
    0 0 0 0 0;
    0 0 0 0 0
];
path4 = astar(grid4, [1 1], [5 5]);
assert(isempty(path4));

% --- コーナーカット禁止の確認（斜めの隙間を突き抜けて到達できないこと） ---
grid5 = [
    0 1 0;
    1 0 1;
    0 1 0
];
path5 = astar(grid5, [1 1], [3 3]);
assert(isempty(path5));

disp('All tests passed.');
