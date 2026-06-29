function path = rrt_star(obstacles, bounds, start, goal, goal_radius, step_size, max_iterations, seed)
% RRT_STAR RRT*（Rapidly-exploring Random Tree Star / 漸近最適RRT）による経路探索
%
%   基本のRRTを改良し、漸近的最適性（ノード数を増やすほど経路コストが最適解に
%   収束する性質）を持たせたサンプリングベースの経路計画アルゴリズム。
%   2次元連続空間上で、startを根とする木をランダムサンプリングによって
%   段階的に広げていき、goal付近まで到達する経路を探索する。各反復で以下を行う:
%     1. 探索空間内からランダムに点（サンプル点）を1つ選ぶ（goalバイアスなし、
%        基本RRTと同じ）。
%     2. 木の中でサンプル点に最も近いノード（最近傍ノード）を見つける。
%     3. 最近傍ノードからサンプル点方向にstep_size分だけ進んだ新しい点を計算する
%        （これを「ステア」と呼ぶ）。
%     4. 最近傍ノードから新しい点への線分が障害物と衝突していなければ次に進む。
%     5. 親選択（choose parent、RRT*独自）: 新しい点から一定の近傍半径以内にある
%        既存ノードを全て探し（近傍集合）、その中から
%        「ノードのコスト（startからの累積距離）+ そのノードから新しい点までの距離」
%        が最小になり、かつ間の線分が衝突しないノードを新しい点の親として選ぶ。
%        これにより、単純な最近傍ノードではなく、より低コストな経路を与える
%        ノードが親になることがある。近傍集合が空、または全て衝突する場合は
%        通常通り最近傍ノードを親にする。なお、親候補・リワイヤ対象として
%        実際に採用するのはstep_size以内のノードに限定している（近傍探索自体は
%        step_sizeより広い半径で行うが、採用条件をstep_size以内に絞ることで、
%        木のどの辺もstep_size以下に保たれ、基本RRTと同じ「1ステップの移動距離は
%        step_size以下」という性質を保てる。本来のRRT*はこの制約を置かないが、
%        経路検証を単純にするための学習用途の簡略化）。
%     6. 新しい点をそのコストとともに木に追加する。
%     7. リワイヤ（rewire、RRT*独自）: 新しいノードの近傍集合（手順5と同じ近傍
%        半径、ただし採用はstep_size以内に限定）にある既存の各ノードについて、
%        「新しいノードのコスト + 新しいノードからその既存ノードまでの距離」が
%        現在のコストより小さく、かつ間の線分が衝突しないなら、その既存ノードの
%        親を新しいノードに付け替えてコストを更新する。この付け替えによって
%        コストが変化した既存ノードの子孫すべてのコストも再帰的に更新する。
%     8. 新しいノードがgoalからgoal_radius以内であれば、その時点で経路を復元
%        して返す。RRT*は本来、漸近的最適性のために全イテレーションを回して
%        最良コストの経路を保持する設計が望ましいが、本実装では学習用途の
%        シンプルさを優先し、goalに最初に到達した時点で経路を即座に返す
%        （基本RRTと同じ「最初に見つかった経路を返す」方式）。より高度な実装
%        では、全イテレーション終了後にgoal近傍の全ノードからコスト最小の
%        ものを選んで経路を復元する。
%
%   時間・空間複雑度:
%     最近傍探索・近傍探索ともに、学習用途のため木の全ノードを線形走査する
%     ナイーブな実装（いずれもO(n)、nは現在の木のノード数）になっている。
%     本格的な実装ではKD-tree等の空間分割構造を使って高速化する。
%     1イテレーションあたり、最近傍探索にO(n)、親選択の近傍探索にO(n)、
%     リワイヤでも近傍ノード分（最大O(n)）の追加処理が必要になるため、
%     1イテレーションあたりO(n)程度。合計でO(n^2)程度（n = max_iterations、
%     最悪ケース）。空間複雑度はノードと親子関係を保持するためO(n)。
%
%   path = RRT_STAR(obstacles, bounds, start, goal, goal_radius, step_size, max_iterations, seed)
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

    % 親選択・リワイヤで使う近傍半径。シンプルな固定値（step_sizeの定数倍）を
    % 採用する。値が大きいほど最適化の効果は高まるが、近傍探索・リワイヤの
    % コストも増える。
    neighbor_radius = step_size * 5.0;

    % ノードはNx4行列として保持: 列1=x, 列2=y, 列3=親ノードのインデックス
    % （startは0）, 列4=startからの累積コスト（距離）。
    nodes = [start(1), start(2), 0, 0.0];

    for iter = 1:max_iterations
        % 1. 探索空間内から完全にランダムな点をサンプリングする（goalバイアスなし）
        sample_x = xmin + rand() * (xmax - xmin);
        sample_y = ymin + rand() * (ymax - ymin);

        % 2. 木の中でサンプル点に最も近いノードを探す（線形探索、O(n)）
        nearest_index = find_nearest_node_index(nodes, sample_x, sample_y);
        nearest_x = nodes(nearest_index, 1);
        nearest_y = nodes(nearest_index, 2);
        nearest_cost = nodes(nearest_index, 4);

        % 3. 最近傍ノードからサンプル点方向にstep_size分だけ伸ばした新しい点を作る
        [new_x, new_y] = steer(nearest_x, nearest_y, sample_x, sample_y, step_size);

        % 4. 新しい点への線分が障害物と衝突していなければ次に進む
        if ~is_segment_collision_free(nearest_x, nearest_y, new_x, new_y, obstacles)
            continue;
        end

        % 5. 親選択（choose parent）: 新しい点の近傍集合の中から、
        %    「ノードのコスト + そのノードから新しい点までの距離」が最小で、
        %    かつ間の線分が衝突しないノードを親として選ぶ
        %    （ただし採用条件はstep_size以内のノードに限定する）。
        neighbors = find_neighbor_indices(nodes, new_x, new_y, neighbor_radius);

        best_parent_index = nearest_index;
        best_cost = nearest_cost + hypot(new_x - nearest_x, new_y - nearest_y);

        for k = 1:length(neighbors)
            candidate_index = neighbors(k);
            cand_x = nodes(candidate_index, 1);
            cand_y = nodes(candidate_index, 2);
            cand_cost = nodes(candidate_index, 4);
            candidate_dist = hypot(new_x - cand_x, new_y - cand_y);
            if candidate_dist > step_size
                continue;
            end
            candidate_cost = cand_cost + candidate_dist;
            if candidate_cost < best_cost && ...
                    is_segment_collision_free(cand_x, cand_y, new_x, new_y, obstacles)
                best_cost = candidate_cost;
                best_parent_index = candidate_index;
            end
        end

        % 6. 新しい点をそのコストとともに木に追加する。
        nodes = [nodes; new_x, new_y, best_parent_index, best_cost];
        new_index = size(nodes, 1);

        % 7. リワイヤ（rewire）: 新しいノードの近傍集合にある既存の各ノードについて、
        %    新しいノード経由のコストが現在のコストより小さく、かつ衝突しないなら
        %    親を新しいノードに付け替え、コストを更新する（子孫のコストも伝播）。
        %    こちらもstep_size以内のノードのみを対象にする（上記と同じ理由）。
        for k = 1:length(neighbors)
            neighbor_index = neighbors(k);
            if neighbor_index == best_parent_index
                continue;
            end
            neigh_x = nodes(neighbor_index, 1);
            neigh_y = nodes(neighbor_index, 2);
            dist_to_neighbor = hypot(new_x - neigh_x, new_y - neigh_y);
            if dist_to_neighbor > step_size
                continue;
            end
            cost_through_new = nodes(new_index, 4) + dist_to_neighbor;
            if cost_through_new < nodes(neighbor_index, 4) && ...
                    is_segment_collision_free(new_x, new_y, neigh_x, neigh_y, obstacles)
                nodes(neighbor_index, 3) = new_index;
                nodes(neighbor_index, 4) = cost_through_new;
                nodes = update_descendant_costs(nodes, neighbor_index);
            end
        end

        % 8. 新しいノードがgoalからgoal_radius以内であれば、その時点で経路を
        %    復元して返す。学習用途のため、本実装ではgoal到達時点で即座に
        %    経路を返す。より高度な実装では全イテレーション後に
        %    最良コストの経路を選ぶ。
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

function neighbors = find_neighbor_indices(nodes, x, y, neighbor_radius)
% 点(x, y)からneighbor_radius以内にある全ノードのインデックスを線形探索で
% 求める（親選択とリワイヤで共通して使う近傍集合）。学習用途のためO(n)実装。
    dx = nodes(:, 1) - x;
    dy = nodes(:, 2) - y;
    dists = hypot(dx, dy);
    neighbors = find(dists <= neighbor_radius);
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

function nodes = update_descendant_costs(nodes, node_index)
% node_indexを親とする子ノードのコストを再帰的に更新する
% （リワイヤによって親ノードのコストが変わったとき、子孫全体に伝播させるため）。
%
% 学習用途のため、親リンクをたどる代わりに毎回全ノードを線形走査して
% 直接の子を探す素朴な実装にしている（O(n)、深い木ではさらに再帰呼び出し分かかる）。
    parent_x = nodes(node_index, 1);
    parent_y = nodes(node_index, 2);
    parent_cost = nodes(node_index, 4);
    children = find(nodes(:, 3) == node_index);
    for k = 1:length(children)
        child_index = children(k);
        nodes(child_index, 4) = parent_cost + hypot(nodes(child_index, 1) - parent_x, ...
            nodes(child_index, 2) - parent_y);
        nodes = update_descendant_costs(nodes, child_index);
    end
end
