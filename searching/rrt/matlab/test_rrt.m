% test_rrt.m
%
% rrt関数のテストスクリプト（MATLAB/Octave両対応）。
% このフォルダを単独で取り出してそのまま実行できる（外部依存なし）。
%
% RRTは乱数を使うアルゴリズムであり、固定シードでもC++/Python/MATLAB間で
% 同じ乱数列にはならない（言語ごとに乱数生成器の実装が異なるため）。
% そのため、ここでは出力の厳密な値ではなく、以下の「性質」を検証する
% プロパティベーステストを行う:
%   1. 十分大きなmax_iterationsであれば、複数の固定シードで経路が見つかる。
%   2. 経路の先頭がstartと一致する。
%   3. 経路の末尾がgoalからgoal_radius以内にある。
%   4. 経路上の各線分が障害物と衝突していない。
%   5. 経路上の各ステップの移動距離がstep_size以下（許容誤差込み）である。
%   6. 障害物による迂回が必要なシナリオでも1〜5が成り立つ。
%   7. max_iterationsが極端に小さい場合は空、または性質を満たす経路のみが返る。
%
% 実行方法: MATLAB/Octave上でこのスクリプトを実行する。
%   >> test_rrt
% 全テストが成功すれば "All tests passed." が表示される。

bounds = [0, 0, 10, 10];
start = [1, 1];
goal = [9, 9];
goal_radius = 0.5;
step_size = 0.5;
max_iterations = 5000;

% 1〜5: 障害物なしのシンプルなシナリオを複数シードで検証
no_obstacles = zeros(0, 3);
for seed = [1, 2, 3]
    path = rrt(no_obstacles, bounds, start, goal, goal_radius, step_size, max_iterations, seed);
    assert_valid_path(path, start, goal, goal_radius, step_size, no_obstacles, true, ...
        sprintf('no obstacles, seed=%d', seed));
end

% 6: start-goal間に大きな円障害物を配置し、迂回が必要なシナリオ
with_obstacle = [5, 5, 2];
for seed = [1, 2, 3]
    path = rrt(with_obstacle, bounds, start, goal, goal_radius, step_size, max_iterations, seed);
    assert_valid_path(path, start, goal, goal_radius, step_size, with_obstacle, true, ...
        sprintf('with obstacle, seed=%d', seed));
end

% 7: max_iterationsが極端に小さい場合（空、または性質を満たす経路のみ許容）
path = rrt(no_obstacles, bounds, start, goal, goal_radius, step_size, 1, 42);
assert_valid_path(path, start, goal, goal_radius, step_size, no_obstacles, false, ...
    'tiny max_iterations');

disp('All tests passed.');

function assert_valid_path(path, start, goal, goal_radius, step_size, obstacles, expect_nonempty, label)
% 経路が満たすべき性質（2〜5）をまとめて検証するヘルパー関数。
    if isempty(path)
        assert(~expect_nonempty, sprintf('%s: path is empty but expected non-empty', label));
        return;
    end

    % 2. 先頭がstartと一致する
    assert(norm(path(1, :) - start) < 1e-9, sprintf('%s: path does not start at start', label));

    % 3. 末尾がgoalからgoal_radius以内にある
    final_dist = norm(path(end, :) - goal);
    assert(final_dist <= goal_radius + 1e-6, ...
        sprintf('%s: final point distance to goal = %f > goal_radius', label, final_dist));

    % 4, 5. 各線分が障害物と衝突していない & 各ステップがstep_size以下
    for i = 1:(size(path, 1) - 1)
        p1 = path(i, :);
        p2 = path(i + 1, :);
        step_dist = norm(p2 - p1);
        assert(step_dist <= step_size * 1.01, ...
            sprintf('%s: step %d distance %f exceeds step_size*1.01', label, i, step_dist));

        for k = 1:size(obstacles, 1)
            cx = obstacles(k, 1);
            cy = obstacles(k, 2);
            radius = obstacles(k, 3);
            d = point_to_segment_distance(cx, cy, p1(1), p1(2), p2(1), p2(2));
            assert(d > radius, sprintf('%s: segment %d collides with obstacle at (%f, %f)', ...
                label, i, cx, cy));
        end
    end
end

function d = point_to_segment_distance(px, py, x1, y1, x2, y2)
% 点(px, py)と線分(x1,y1)-(x2,y2)との最短距離（テスト側の独立実装）。
    dx = x2 - x1;
    dy = y2 - y1;
    length_squared = dx^2 + dy^2;
    if length_squared < 1e-12
        d = hypot(px - x1, py - y1);
        return;
    end
    t = ((px - x1) * dx + (py - y1) * dy) / length_squared;
    t = max(0, min(1, t));
    closest_x = x1 + t * dx;
    closest_y = y1 + t * dy;
    d = hypot(px - closest_x, py - closest_y);
end
