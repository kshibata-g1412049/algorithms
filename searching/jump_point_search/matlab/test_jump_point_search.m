% test_jump_point_search.m
%
% jump_point_search関数のテストスクリプト（MATLAB/Octave両対応）。
% このフォルダを単独で取り出してそのまま実行できる（外部依存なし）。
%
% 実行方法: MATLAB/Octave上でこのスクリプトを実行する。
%   >> test_jump_point_search
% 全テストが成功すれば "All tests passed." が表示される。

sqrt2 = sqrt(2);
eps_tol = 1e-6;

% --- 障害物なしの直線（斜め）経路。最短コストは4*sqrt(2) ---
grid1 = zeros(5, 5);
startPos = [1 1];
goalPos = [5 5];
path1 = jump_point_search(grid1, startPos, goalPos);
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
path2 = jump_point_search(grid2, startPos, goalPos);
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
path3 = jump_point_search(grid3, [2 2], [2 2]);
assert(isequal(path3, [2 2]));

% --- 到達不可能なケース（完全に壁で塞がれている） ---
grid4 = [
    0 0 0 0 0;
    1 1 1 1 1;
    0 0 0 0 0;
    0 0 0 0 0;
    0 0 0 0 0
];
path4 = jump_point_search(grid4, [1 1], [5 5]);
assert(isempty(path4));

% --- コーナーカット禁止の確認（斜めの隙間を突き抜けて到達できないこと） ---
grid5 = [
    0 1 0;
    1 0 1;
    0 1 0
];
path5 = jump_point_search(grid5, [1 1], [3 3]);
assert(isempty(path5));

% --- 大きめのランダム障害物グリッド。期待コスト13は独立実装のDijkstraで
%     事前に計算・確認済み ---
grid6 = [
    0 0 0 0 0 0 0 0;
    0 1 1 0 1 1 1 0;
    0 1 0 0 0 0 1 0;
    0 1 0 1 1 0 1 0;
    0 0 0 1 0 0 0 0;
    0 1 1 1 0 1 1 0;
    0 0 0 0 0 0 0 0
];
startPos = [1 1];
goalPos = [7 8];
path6 = jump_point_search(grid6, startPos, goalPos);
assert(~isempty(path6));
assert(isequal(path6(1, :), startPos) && isequal(path6(end, :), goalPos));
cost6 = 0;
for i = 2:size(path6, 1)
    r = path6(i, 1);
    c = path6(i, 2);
    assert(grid6(r, c) == 0);
    dr = path6(i, 1) - path6(i - 1, 1);
    dc = path6(i, 2) - path6(i - 1, 2);
    assert(abs(dr) <= 1 && abs(dc) <= 1 && ~(dr == 0 && dc == 0));
    if dr ~= 0 && dc ~= 0
        r0 = path6(i - 1, 1);
        c0 = path6(i - 1, 2);
        assert(grid6(r0 + dr, c0) == 0 && grid6(r0, c0 + dc) == 0);
        cost6 = cost6 + sqrt2;
    else
        cost6 = cost6 + 1;
    end
end
assert(abs(cost6 - 13.0) < eps_tol);

% --- コーナーカット禁止下での「強制されていない」斜めショートカットが
%     必要なケース。地点(2,3)（1始まり）で直進中に斜めへ折れないと
%     最短コスト 11+sqrt(2) を達成できない ---
grid7 = [
    0 0 1 1 1 0;
    0 0 0 0 0 1;
    0 1 0 0 0 0;
    1 1 1 0 0 1;
    0 1 0 0 1 1;
    0 1 0 0 0 0;
    1 0 1 0 1 1;
    0 0 1 0 0 0;
    0 0 0 0 0 0;
    0 0 1 0 1 0
];
startPos = [1 2];
goalPos = [9 1];
path7 = jump_point_search(grid7, startPos, goalPos);
assert(~isempty(path7));
assert(isequal(path7(1, :), startPos) && isequal(path7(end, :), goalPos));
cost7 = 0;
for i = 2:size(path7, 1)
    r = path7(i, 1);
    c = path7(i, 2);
    assert(grid7(r, c) == 0);
    dr = path7(i, 1) - path7(i - 1, 1);
    dc = path7(i, 2) - path7(i - 1, 2);
    assert(abs(dr) <= 1 && abs(dc) <= 1 && ~(dr == 0 && dc == 0));
    if dr ~= 0 && dc ~= 0
        r0 = path7(i - 1, 1);
        c0 = path7(i - 1, 2);
        assert(grid7(r0 + dr, c0) == 0 && grid7(r0, c0 + dc) == 0);
        cost7 = cost7 + sqrt2;
    else
        cost7 = cost7 + 1;
    end
end
assert(abs(cost7 - (11.0 + sqrt2)) < eps_tol);

disp('All tests passed.');
