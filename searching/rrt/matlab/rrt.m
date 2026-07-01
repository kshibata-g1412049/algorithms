function path = rrt(obstacles, bounds, start, goal, goal_radius, step_size, max_iterations, seed)
% RRT 高速探索ランダム木（Rapidly-exploring Random Tree）による経路探索
%
%   2次元連続空間上で、startを根とする木をランダムサンプリングによって
%   段階的に広げていき、goal付近まで到達する経路を探索するサンプリングベースの
%   経路計画アルゴリズム。各反復で以下を行う:
%     1. 探索空間内からランダムに点（サンプル点）を1つ選ぶ。
%     2. 木の中でサンプル点に最も近いノード（最近傍ノード）を見つける。
%     3. 最近傍ノードからサンプル点方向にstep_size分だけ進んだ新しいノードを作る
%        （これを「ステア」と呼ぶ）。
%     4. 最近傍ノードから新しいノードへの線分が障害物と衝突しなければ、
%        新しいノードを木に追加する。
%     5. 新しいノードがgoalからgoal_radius以内であれば探索を終了し、
%        startから新しいノードまでの経路を木の親リンクをたどって復元する。
%
%   基本のRRTにはgoal方向へのサンプリングバイアスは無く、サンプル点は探索空間内に
%   完全にランダムに選ばれる（goal_biased_rrtとの違いはこの点のみ）。
%
%   時間・空間複雑度:
%     最近傍探索は、学習用途のため木の全ノードを線形走査するナイーブな実装
%     （O(n)、nは現在の木のノード数）になっている。本格的な実装ではKD-tree等の
%     空間分割構造を使って最近傍探索をO(log n)程度に高速化する。
%     1イテレーションあたりO(n)の最近傍探索を行うため、合計でO(n^2)程度
%     （n = max_iterations、最悪ケース）。空間複雑度はノードを保持するためO(n)。
%
%   path = RRT(obstacles, bounds, start, goal, goal_radius, step_size, max_iterations, seed)
%
%   obstacles: N×3行列。各行が[center_x, center_y, radius]。
%   bounds: 探索空間の矩形領域 [xmin, ymin, xmax, ymax]。
%   start: 探索開始点 [x, y]。
%   goal: 目標点 [x, y]。
%   goal_radius: goalからこの半径以内に到達したら成功とみなす。
%   step_size: 1ステップで木を伸ばす距離。
%   max_iterations: 最大反復回数。これを超えても到達できなければ空行列[]を返す。
%   seed: 乱数生成器を初期化するシード値（再現性確保のため必須）。
%
%   path: M×2行列。startが1行目、goal付近の到達点が最終行。
%         見つからない場合は空行列[]。

    rng(seed);

    xmin = bounds(1); ymin = bounds(2);
    xmax = bounds(3); ymax = bounds(4);

    % ノードはNx3行列として保持: 列1=x, 列2=y, 列3=親ノードのインデックス（startは0）
    nodes = [start(1), start(2), 0];

    for iter = 1:max_iterations
        % 1. 探索空間内から完全にランダムな点をサンプリングする（goalバイアスなし）
        sample_x = xmin + rand() * (xmax - xmin);
        sample_y = ymin + rand() * (ymax - ymin);

        % 2. 木の中でサンプル点に最も近いノードを探す（線形探索、O(n)）
        nearest_index = find_nearest_node_index(nodes, sample_x, sample_y);
        nearest_x = nodes(nearest_index, 1);
        nearest_y = nodes(nearest_index, 2);

        % 3. 最近傍ノードからサンプル点方向にstep_size分だけ伸ばした新しい点を作る
        [new_x, new_y] = steer(nearest_x, nearest_y, sample_x, sample_y, step_size);

        % 4. 新しい点への線分が障害物と衝突していなければ木に追加する
        if ~is_segment_collision_free(nearest_x, nearest_y, new_x, new_y, obstacles)
            continue;
        end

        nodes = [nodes; new_x, new_y, nearest_index];
        new_index = size(nodes, 1);

        % 5. 新しいノードがgoalからgoal_radius以内であれば経路を復元して返す
        if hypot(new_x - goal(1), new_y - goal(2)) <= goal_radius
            path = reconstruct_path(nodes, new_index);
            return;
        end
    end

    % max_iterations以内にgoalへ到達できなかった
    path = [];
end

function nearest_index = find_nearest_node_index(nodes, x, y)
% 木の中でサンプル点(x, y)に最も近いノードのインデックスを線形探索で求める。
% 学習用途のためナイーブなO(n)実装。本格的な実装ではKD-tree等で高速化する。
    dx = nodes(:, 1) - x;
    dy = nodes(:, 2) - y;
    dists = hypot(dx, dy);
    [~, nearest_index] = min(dists);
end

function [new_x, new_y] = steer(from_x, from_y, toward_x, toward_y, step_size)
% fromノードからtoward方向にstep_size分だけ進んだ新しい点を計算する
% （toward自体がstep_size以内にある場合はtoward自体を返す）。
    dist = hypot(toward_x - from_x, toward_y - from_y);
    if dist <= step_size
        new_x = toward_x;
        new_y = toward_y;
    else
        ratio = step_size / dist;
        new_x = from_x + (toward_x - from_x) * ratio;
        new_y = from_y + (toward_y - from_y) * ratio;
    end
end

function free = is_segment_collision_free(x1, y1, x2, y2, obstacles)
% 線分(x1,y1)-(x2,y2)が全ての障害物のいずれとも衝突しないかを判定する。
    free = true;
    for i = 1:size(obstacles, 1)
        cx = obstacles(i, 1);
        cy = obstacles(i, 2);
        radius = obstacles(i, 3);
        if point_to_segment_distance(cx, cy, x1, y1, x2, y2) <= radius
            free = false;
            return;
        end
    end
end

function d = point_to_segment_distance(px, py, x1, y1, x2, y2)
% 点(px, py)と線分(x1,y1)-(x2,y2)との最短距離を求める。
    dx = x2 - x1;
    dy = y2 - y1;
    length_squared = dx^2 + dy^2;

    if length_squared < 1e-12
        % 線分の長さがほぼ0 = 1点とみなして点間距離を返す
        d = hypot(px - x1, py - y1);
        return;
    end

    % 線分上の最近点を表すパラメータt（[0,1]にクランプ）を求める
    t = ((px - x1) * dx + (py - y1) * dy) / length_squared;
    t = max(0, min(1, t));

    closest_x = x1 + t * dx;
    closest_y = y1 + t * dy;
    d = hypot(px - closest_x, py - closest_y);
end

function path = reconstruct_path(nodes, node_index)
% 木の中で、goalノードからnode_indexまで親リンクをたどって経路を復元する。
    path = [];
    current = node_index;
    while current ~= 0
        path = [nodes(current, 1), nodes(current, 2); path];
        current = nodes(current, 3);
    end
end
