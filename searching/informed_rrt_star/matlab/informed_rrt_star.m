function path = informed_rrt_star(obstacles, bounds, start, goal, goal_radius, step_size, max_iterations, seed)
% INFORMED_RRT_STAR Informed RRT*（Informed Rapidly-exploring Random Tree Star）
%
%   RRT*（最近傍探索、ステア、親選択(choose parent)、リワイヤ(rewire)）をベースに、
%   「最初の解が見つかった後はサンプリング範囲を絞り込む」ことで収束を速くした
%   改良版のサンプリングベース経路計画アルゴリズム。各反復で以下を行う:
%     1. サンプリング: まだ解（startからgoalまでの経路）が見つかっていない間は、
%        RRT*と同様に探索空間全体からランダムに点をサンプリングする（goalバイアス
%        なし）。解が見つかった後は、startとgoalを焦点(foci)とする楕円
%        （詳細は後述）の内部からサンプリングする。これがInformed RRT*の核心
%        である「情報を活用したサンプリング(informed sampling)」。
%     2. 木の中でサンプル点に最も近いノード（最近傍ノード）を見つける。
%     3. 最近傍ノードからサンプル点方向にstep_size分だけ進んだ新しい点を計算する
%        （これを「ステア」と呼ぶ）。
%     4. 最近傍ノードから新しい点への線分が障害物と衝突していなければ次に進む。
%     5. 親選択（choose parent、RRT*と同じ）: 新しい点から一定の近傍半径以内に
%        ある既存ノードを全て探し（近傍集合）、その中から
%        「ノードのコスト（startからの累積距離）+ そのノードから新しい点までの距離」
%        が最小になり、かつ間の線分が衝突しないノードを新しい点の親として選ぶ。
%        近傍集合が空、または全て衝突する場合は通常通り最近傍ノードを親にする。
%        親候補・リワイヤ対象として実際に採用するのはstep_size以内のノードに
%        限定している（近傍探索自体はstep_sizeより広い半径で行うが、採用条件を
%        step_size以内に絞ることで、木のどの辺もstep_size以下に保たれ、基本RRTと
%        同じ「1ステップの移動距離はstep_size以下」という性質を保てる。本来の
%        RRT*はこの制約を置かないが、経路検証を単純にするための学習用途の
%        簡略化）。
%     6. 新しい点をそのコストとともに木に追加する。
%     7. リワイヤ（rewire、RRT*と同じ）: 新しいノードの近傍集合（手順5と同じ
%        近傍半径、ただし採用はstep_size以内に限定）にある既存の各ノードについて、
%        「新しいノードのコスト + 新しいノードからその既存ノードまでの距離」が
%        現在のコストより小さく、かつ間の線分が衝突しないなら、その既存ノードの
%        親を新しいノードに付け替えてコストを更新する。この付け替えによって
%        コストが変化した既存ノードの子孫すべてのコストも再帰的に更新する。
%     8. 新しいノードがgoalからgoal_radius以内であれば、startからそのノードまでの
%        コストを計算し、これまでの最良コスト(best_cost)より小さければ、
%        最良経路・最良コストを更新する。これによりサンプリング楕円
%        （後述）が縮小し、以降の探索がより効率的になる。本実装では学習用途の
%        シンプルさを優先しつつも、Informed RRT*の本質である
%        「最初の解の後も探索を続けて改善する」ことを実演するため、
%        最初の解が見つかった後もmax_iterations回まで探索を継続し、見つかる
%        たびにより良い経路で更新し続ける。最終的に見つかった最良の経路を返す
%        （最初に見つかった経路をすぐ返す設計ではない）。
%
%   楕円サンプリング（informed sampling）の詳細:
%     最初の解が見つかった後のサンプリング範囲は、startとgoalを焦点とする
%     楕円に限定される。
%       c_min  = startとgoalのユークリッド距離（楕円の長軸方向の最小値。
%                これより縮めることはできない＝直線距離が経路長の下限のため）。
%       c_best = これまでに見つかった最良経路のコスト（累積距離）。
%       長軸半径 a = c_best / 2
%       短軸半径 b = sqrt(c_best^2 - c_min^2) / 2
%       楕円の中心 = startとgoalの中点。
%       楕円の回転角 = start->goalの方向角（atan2）。
%     サンプリング手順: 単位円内の一様ランダムな点を、rejection sampling
%     （[-1,1]^2の正方形内に点を打ち、単位円内に入るまで繰り返す）で生成し、
%     それを(a, b)でスケーリングし、回転角で回転し、楕円の中心に平行移動して
%     サンプル点を得る。生成したサンプル点が探索空間のbounds外に出る場合は、
%     再サンプリングする（本実装の方針。bounds内に収まる点が得られるまで
%     ループする。理論上は稀だが無限ループを避けるため上限回数を設け、
%     上限に達したら通常のbounds全体からの一様サンプリングにフォールバックする）。
%     c_bestが更新されるたびに楕円が縮小し、以降のサンプリングがより
%     startからgoalへの直線に近い領域に集中するようになる。
%
%   時間・空間複雑度:
%     最近傍探索・近傍探索ともに、学習用途のため木の全ノードを線形走査する
%     ナイーブな実装（いずれもO(n)、nは現在の木のノード数）になっている。
%     本格的な実装ではKD-tree等の空間分割構造を使って高速化する。
%     1イテレーションあたり、最近傍探索にO(n)、親選択の近傍探索にO(n)、
%     リワイヤでも近傍ノード分（最大O(n)）の追加処理が必要になるため、
%     1イテレーションあたりO(n)程度。合計でO(n^2)程度（n = max_iterations、
%     最悪ケース）。楕円サンプリングのrejection samplingは定数回（期待値）で
%     完了するため漸近的な複雑度に影響しない。空間複雑度はノードと親子関係を
%     保持するためO(n)。
%
%   path = INFORMED_RRT_STAR(obstacles, bounds, start, goal, goal_radius, ...
%       step_size, max_iterations, seed)
%
%   obstacles: N×3行列。各行が[center_x, center_y, radius]。
%   bounds: 探索空間の矩形領域 [xmin, ymin, xmax, ymax]。
%   start: 探索開始点 [x, y]。
%   goal: 目標点 [x, y]。
%   goal_radius: goalからこの半径以内に到達したら成功とみなす。
%   step_size: 1ステップで木を伸ばす距離。
%   max_iterations: 最大反復回数。常にこの回数まで探索し、見つかった中で
%       最良コストの経路を返す（RRT/RRT*と異なり最初の解で打ち切らない）。
%   seed: 乱数生成器を初期化するシード値（再現性確保のため必須）。
%
%   path: M×2行列。startが1行目、goal付近の到達点が最終行（見つかった中で
%         最良コストの経路）。見つからない場合は空行列[]。

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

    % informed samplingの楕円パラメータ。
    c_min = hypot(goal(1) - start(1), goal(2) - start(2));
    center_x = (start(1) + goal(1)) / 2.0;
    center_y = (start(2) + goal(2)) / 2.0;
    angle = atan2(goal(2) - start(2), goal(1) - start(1));

    % これまでに見つかった最良経路のコストとgoal側のノードインデックス。
    % 解が見つかるまではbest_costは無限大とし、探索空間全体からサンプリングする。
    best_cost = Inf;
    best_goal_index = 0; % 0は「まだ見つかっていない」ことを表すセンチネル

    % 楕円サンプリングがboundsの外に出た場合の再サンプリング上限回数。
    % 上限に達したら通常のbounds全体からの一様サンプリングにフォールバックする。
    max_ellipse_resample_attempts = 100;

    for iter = 1:max_iterations
        % 1. サンプリング: 解が見つかっていなければ探索空間全体から、
        %    見つかっていれば（informed sampling）start-goalを焦点とする
        %    楕円（best_costに応じて縮小していく）からサンプリングする。
        if ~isfinite(best_cost)
            sample_x = xmin + rand() * (xmax - xmin);
            sample_y = ymin + rand() * (ymax - ymin);
        else
            sampled = false;
            for attempt = 1:max_ellipse_resample_attempts
                [point_x, point_y] = sample_from_ellipse(c_min, best_cost, ...
                    center_x, center_y, angle);
                if point_x >= xmin && point_x <= xmax && ...
                        point_y >= ymin && point_y <= ymax
                    sample_x = point_x;
                    sample_y = point_y;
                    sampled = true;
                    break;
                end
            end
            if ~sampled
                % 楕円からbounds内の点が得られなかった場合のフォールバック。
                sample_x = xmin + rand() * (xmax - xmin);
                sample_y = ymin + rand() * (ymax - ymin);
            end
        end

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
        best_parent_cost = nearest_cost + hypot(new_x - nearest_x, new_y - nearest_y);

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
            if candidate_cost < best_parent_cost && ...
                    is_segment_collision_free(cand_x, cand_y, new_x, new_y, obstacles)
                best_parent_cost = candidate_cost;
                best_parent_index = candidate_index;
            end
        end

        % 6. 新しい点をそのコストとともに木に追加する。
        nodes = [nodes; new_x, new_y, best_parent_index, best_parent_cost];
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

        % 8. 新しいノードがgoalからgoal_radius以内であれば、そのコストが
        %    これまでの最良コストより小さければ最良経路・最良コストを更新する。
        %    これによってinformed samplingの楕円が縮小し、以降の探索が
        %    より効率的になる。Informed RRT*の本質を実演するため、最初の解が
        %    見つかった後もmax_iterations回まで探索を継続し、より良い経路が
        %    見つかれば更新し続ける（最初に見つかった経路を即座に返す設計とは
        %    異なる）。
        if hypot(new_x - goal(1), new_y - goal(2)) <= goal_radius
            if nodes(new_index, 4) < best_cost
                best_cost = nodes(new_index, 4);
                best_goal_index = new_index;
            end
        end
    end

    % max_iterations回探索した結果、見つかった中で最良の経路を返す
    % （一度も到達できなければ空行列を返す）。
    if best_goal_index == 0
        path = [];
    else
        path = reconstruct_path(nodes, best_goal_index);
    end
end

function [x, y] = sample_from_ellipse(c_min, c_best, center_x, center_y, angle)
% startとgoalを焦点とする楕円（informed sampling用）から1点をサンプリングする。
%
%   c_min: startとgoalのユークリッド距離（楕円の長軸方向の最小値）。
%   c_best: 現在の最良経路コスト。
%   center_x, center_y: 楕円の中心（startとgoalの中点）。
%   angle: 楕円の回転角（start->goalの方向角）。
%
%   手順: 単位円内の点をrejection samplingで得て、(a, b)でスケーリングし、
%   回転・平行移動して楕円上の点に変換する。
    a = c_best / 2.0;
    b_squared = (c_best^2 - c_min^2) / 4.0;
    b = sqrt(max(0.0, b_squared));

    % 単位円内の一様ランダムな点をrejection samplingで生成する。
    while true
        ux = -1.0 + 2.0 * rand();
        uy = -1.0 + 2.0 * rand();
        if ux^2 + uy^2 <= 1.0
            break;
        end
    end

    % 単位円上の点を楕円の形状（半径a, b）にスケーリングする。
    ellipse_x = a * ux;
    ellipse_y = b * uy;

    % 回転（start->goal方向に合わせる）してから中心へ平行移動する。
    cos_angle = cos(angle);
    sin_angle = sin(angle);
    rotated_x = ellipse_x * cos_angle - ellipse_y * sin_angle;
    rotated_y = ellipse_x * sin_angle + ellipse_y * cos_angle;

    x = rotated_x + center_x;
    y = rotated_y + center_y;
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
