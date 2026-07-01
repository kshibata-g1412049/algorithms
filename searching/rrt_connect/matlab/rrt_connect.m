function path = rrt_connect(obstacles, bounds, start, goal, goal_radius, step_size, max_iterations, seed)
% RRT_CONNECT RRT-Connect（双方向RRT / Bidirectional Rapidly-exploring Random Tree）による経路探索
%
%   startを根とする木（tree_a）とgoalを根とする木（tree_b）の2本を同時に育て、
%   互いに向かって伸ばし合うことで通常のRRTより高速にstart-goal間の経路を
%   見つけるサンプリングベースの経路計画アルゴリズム。各反復で以下を行う:
%     1. 探索空間内からランダムに点（サンプル点）を1つ選ぶ（goalバイアスなし）。
%     2. tree_aに対して「extend」処理を行う: tree_a内の最近傍ノードを見つけ、
%        サンプル点方向にstep_size分ステアし、衝突しなければ新しいノードを
%        tree_aに追加する（基本RRTの1ステップと同じ処理）。
%     3. extendが成功したら、その新しいノードに向けてtree_bから「connect」を
%        試みる: tree_b内の最近傍ノードから新しいノードに向かって、衝突しない
%        限りstep_size刻みで繰り返しステアし続ける（targetに到達するか、
%        衝突するか、ステア回数の上限に達するまで）。これによりtree_bが
%        tree_aの新しいノードに「届く」可能性がある。
%     4. 2つの木のノードがgoal_radius以内（またはstep_size以内）まで近づいたら
%        接続成功とみなし、tree_a側の経路（root→接続点）とtree_b側の経路
%        （接続点→root）を結合してstart→goalの完全な経路を構築する。
%        tree_bはgoalを根にしているため、tree_bの経路は反転させてから連結する。
%     5. 次のイテレーションではtree_aとtree_bの役割を交換し、両方の木が
%        均等に成長するようにする。
%     6. max_iterations以内に接続できなければ空行列[]を返す。
%
%   厳密なRRT-Connect論文のtrapped/advanced状態遷移は再現していない簡略版だが、
%   「両方の木が交互に伸び、互いに接続を試み、接続できたら経路を結合する」と
%   いう核心的な振る舞いは実装している。connectのステア繰り返し回数には
%   上限を設けて無限ループを避けている。
%
%   時間・空間複雑度:
%     最近傍探索は、学習用途のため木の全ノードを線形走査するナイーブな実装
%     （O(n)、nは木のノード数）になっている。本格的な実装ではKD-tree等の
%     空間分割構造を使って最近傍探索をO(log n)程度に高速化する。
%     1イテレーションではextendで最近傍探索1回（O(n)）、connectで複数回の
%     ステア・最近傍探索（定数倍はconnectの繰り返し回数の上限）を行うため
%     1イテレーションあたりO(n)程度。合計でO(n^2)程度
%     （n = max_iterations、最悪ケース）。空間複雑度は2本の木のノードを
%     保持するためO(n)。
%
%   path = RRT_CONNECT(obstacles, bounds, start, goal, goal_radius, step_size, max_iterations, seed)
%
%   obstacles: N×3行列。各行が[center_x, center_y, radius]。
%   bounds: 探索空間の矩形領域 [xmin, ymin, xmax, ymax]。
%   start: 探索開始点 [x, y]。
%   goal: 目標点 [x, y]。
%   goal_radius: 2つの木のノードがこの半径以内まで近づいたら接続成功とみなす。
%   step_size: 1ステップで木を伸ばす距離。
%   max_iterations: 最大反復回数。これを超えても接続できなければ空行列[]を返す。
%   seed: 乱数生成器を初期化するシード値（再現性確保のため必須）。
%
%   path: M×2行列。startが1行目、goal付近の到達点が最終行。
%         見つからない場合は空行列[]。

    rng(seed);

    xmin = bounds(1); ymin = bounds(2);
    xmax = bounds(3); ymax = bounds(4);

    % connectでのステア繰り返し回数の上限。探索空間の対角線長をstep_sizeで
    % 割った値（往復しても十分な余裕を持たせるため2倍）を基準にする。
    diagonal = hypot(xmax - xmin, ymax - ymin);
    max_steer_steps = max(100, floor(2.0 * diagonal / step_size) + 10);

    % ノードはNx3行列として保持: 列1=x, 列2=y, 列3=親ノードのインデックス
    % （根ノードは0）。tree_a = startを根とする木、tree_b = goalを根とする木。
    tree_a = [start(1), start(2), 0];
    tree_b = [goal(1), goal(2), 0];

    % swappedがtrueのとき、tree_aが実際にはgoal側の木である
    % （経路結合時にどちらをstart側として扱うか判定するために使う）
    swapped = false;

    for iter = 1:max_iterations
        % 1. 探索空間内から完全にランダムな点をサンプリングする（goalバイアスなし）
        sample_x = xmin + rand() * (xmax - xmin);
        sample_y = ymin + rand() * (ymax - ymin);

        % 2. tree_aに対してextend処理を行う（基本RRTの1ステップと同じ）
        [tree_a, new_index_a] = extend(tree_a, sample_x, sample_y, step_size, obstacles);

        if new_index_a ~= 0
            % 3. extendが成功したので、tree_bから新しいノードへconnectを試みる
            target_x = tree_a(new_index_a, 1);
            target_y = tree_a(new_index_a, 2);

            [tree_b, new_index_b, reached] = connect(tree_b, target_x, target_y, ...
                step_size, goal_radius, max_steer_steps, obstacles);

            if reached && new_index_b ~= 0
                % 4. 2つの木が接続できた。tree_a側の経路とtree_b側の経路を結合する。
                % path_to_root(tree, idx)はidx→...→rootの順を返す。
                % tree_aの経路（idx=new_index_a→...→tree_aのroot）
                path_a = path_to_root(tree_a, new_index_a);
                % tree_bの経路（idx=new_index_b→...→tree_bのroot）
                path_b = path_to_root(tree_b, new_index_b);

                % start→...→goalの順になるよう、それぞれの根がどちらかに
                % 応じて反転するかどうかを決める。
                if ~swapped
                    % tree_a=start側（root=start）、tree_b=goal側（root=goal）
                    % path_a: new_index_a→...→start なので反転してstart→new_index_aに、
                    % path_b: new_index_b→...→goal はそのまま末尾に連結する。
                    path_a = flipud(path_a);
                    path = [path_a; path_b];
                else
                    % tree_a=goal側（root=goal）、tree_b=start側（root=start）
                    % path_b: new_index_b→...→start なので反転してstart→new_index_bに、
                    % path_a: new_index_a→...→goal はそのまま末尾に連結する。
                    path_b = flipud(path_b);
                    path = [path_b; path_a];
                end
                return;
            end
        end

        % 6. 次のイテレーションのためにtree_aとtree_bの役割を交換する
        tmp = tree_a;
        tree_a = tree_b;
        tree_b = tmp;
        swapped = ~swapped;
    end

    % max_iterations以内に接続できなかった
    path = [];
end

function [tree, new_index] = extend(tree, sample_x, sample_y, step_size, obstacles)
% extend処理: treeに対して、サンプル点(sample_x, sample_y)方向へ1ステップだけ
% 伸ばす。最近傍ノードを探し、ステアし、衝突しなければ新しいノードをtreeに
% 追加する。追加した場合は新ノードのインデックスを返し、衝突した場合は
% new_index=0を返す（基本RRTの1反復と同じ処理）。
    nearest_index = find_nearest_node_index(tree, sample_x, sample_y);
    nearest_x = tree(nearest_index, 1);
    nearest_y = tree(nearest_index, 2);

    [new_x, new_y] = steer(nearest_x, nearest_y, sample_x, sample_y, step_size);

    if ~is_segment_collision_free(nearest_x, nearest_y, new_x, new_y, obstacles)
        new_index = 0;
        return;
    end

    tree = [tree; new_x, new_y, nearest_index];
    new_index = size(tree, 1);
end

function [tree, last_added_index, reached_target] = connect(tree, target_x, target_y, ...
        step_size, goal_radius, max_steer_steps, obstacles)
% connect処理: treeの最近傍ノードから、target_x, target_yに向かって、
% 衝突しない限りstep_size刻みで繰り返しステアし続ける。targetに到達するか、
% 衝突するか、ステア回数の上限に達するまで継続する。
% last_added_index: 最終的にtreeに追加された最後のノードのインデックス
%                    （1つも追加できなかった場合は0）。
% reached_target: targetまでgoal_radius以内に到達できたかどうか。
    reached_target = false;
    last_added_index = 0;

    for step = 1:max_steer_steps
        [tree, new_index] = extend(tree, target_x, target_y, step_size, obstacles);
        if new_index == 0
            % 衝突して伸ばせなかった（trapped）。connectを中断する。
            break;
        end
        last_added_index = new_index;

        d = hypot(tree(new_index, 1) - target_x, tree(new_index, 2) - target_y);
        if d <= goal_radius
            reached_target = true;
            break;
        end
        % d <= step_sizeの場合、次のステアでtarget自体に到達する想定。
        % そのままループを継続してもう一度ステアする。
    end
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

function path = path_to_root(nodes, node_index)
% 木の中で、node_indexから親リンクをたどって根までの経路を復元する
% （根→node_indexの順ではなく、node_index→根の順で返す。呼び出し側で
%  必要に応じて反転する）。
    path = [];
    current = node_index;
    while current ~= 0
        path = [path; nodes(current, 1), nodes(current, 2)];
        current = nodes(current, 3);
    end
end
