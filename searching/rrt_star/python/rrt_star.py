"""RRT*（Rapidly-exploring Random Tree Star / 漸近最適RRT）の実装。

基本のRRTを改良し、漸近的最適性（ノード数を増やすほど経路コストが最適解に
収束する性質）を持たせたサンプリングベースの経路計画アルゴリズム。
2次元連続空間上で、startを根とする木をランダムサンプリングによって
段階的に広げていき、goal付近まで到達する経路を探索する。各反復で以下を行う:
  1. 探索空間内からランダムに点（サンプル点）を1つ選ぶ（goalバイアスなし、
     基本RRTと同じ）。
  2. 木の中でサンプル点に最も近いノード（最近傍ノード）を見つける。
  3. 最近傍ノードからサンプル点方向にstep_size分だけ進んだ新しい点を計算する
     （これを「ステア」と呼ぶ）。
  4. 最近傍ノードから新しい点への線分が障害物と衝突していなければ次に進む。
  5. 親選択（choose parent、RRT*独自）: 新しい点から一定の近傍半径以内にある
     既存ノードを全て探し（近傍集合）、その中から
     「ノードのコスト（startからの累積距離）+ そのノードから新しい点までの距離」
     が最小になり、かつ間の線分が衝突しないノードを新しい点の親として選ぶ。
     これにより、単純な最近傍ノードではなく、より低コストな経路を与える
     ノードが親になることがある。近傍集合が空、または全て衝突する場合は
     通常通り最近傍ノードを親にする。なお、親候補・リワイヤ対象として
     実際に採用するのはstep_size以内のノードに限定している（近傍探索自体は
     step_sizeより広い半径で行うが、採用条件をstep_size以内に絞ることで、
     木のどの辺もstep_size以下に保たれ、基本RRTと同じ「1ステップの移動距離は
     step_size以下」という性質を保てる。本来のRRT*はこの制約を置かないが、
     経路検証を単純にするための学習用途の簡略化）。
  6. 新しい点をそのコストとともに木に追加する。
  7. リワイヤ（rewire、RRT*独自）: 新しいノードの近傍集合（手順5と同じ近傍半径、
     ただし採用はstep_size以内に限定）にある既存の各ノードについて、
     「新しいノードのコスト + 新しいノードからその既存ノードまでの距離」が
     現在のコストより小さく、かつ間の線分が衝突しないなら、その既存ノードの
     親を新しいノードに付け替えてコストを更新する。この付け替えによって
     コストが変化した既存ノードの子孫すべてのコストも再帰的に更新する。
  8. 新しいノードがgoalからgoal_radius以内であれば、その時点で経路を復元して
     返す。RRT*は本来、漸近的最適性のために全イテレーションを回して最良コスト
     の経路を保持する設計が望ましいが、本実装では学習用途のシンプルさを優先し、
     goalに最初に到達した時点で経路を即座に返す（基本RRTと同じ「最初に見つ
     かった経路を返す」方式）。より高度な実装では、全イテレーション終了後に
     goal近傍の全ノードからコスト最小のものを選んで経路を復元する。

時間・空間複雑度:
  最近傍探索・近傍探索ともに、学習用途のため木の全ノードを線形走査する
  ナイーブな実装（いずれもO(n)、nは現在の木のノード数）になっている。
  本格的な実装ではKD-tree等の空間分割構造を使って高速化する。
  1イテレーションあたり、最近傍探索にO(n)、親選択の近傍探索にO(n)、
  リワイヤでも近傍ノード分（最大O(n)）の追加処理が必要になるため、
  1イテレーションあたりO(n)程度。合計でO(n^2)程度（n = max_iterations、
  最悪ケース）。空間複雑度はノードと親子関係を保持するためO(n)。
"""
import math
import random
from typing import List, Tuple


class _Node:
    """木の1ノード。parent_indexで親ノードへのリンク、costでstartからの
    累積コスト（距離）を保持する。
    """

    __slots__ = ("x", "y", "parent_index", "cost")

    def __init__(self, x: float, y: float, parent_index: int, cost: float):
        self.x = x
        self.y = y
        self.parent_index = parent_index  # startノードは-1
        self.cost = cost


def _distance(x1: float, y1: float, x2: float, y2: float) -> float:
    """2点間のユークリッド距離。"""
    return math.hypot(x2 - x1, y2 - y1)


def _point_to_segment_distance(px: float, py: float, x1: float, y1: float,
                                x2: float, y2: float) -> float:
    """点(px, py)と線分(x1,y1)-(x2,y2)との最短距離を求める。"""
    dx = x2 - x1
    dy = y2 - y1
    length_squared = dx * dx + dy * dy

    if length_squared < 1e-12:
        # 線分の長さがほぼ0 = 1点とみなして点間距離を返す
        return _distance(px, py, x1, y1)

    # 線分上の最近点を表すパラメータt（[0,1]にクランプ）を求める
    t = ((px - x1) * dx + (py - y1) * dy) / length_squared
    t = max(0.0, min(1.0, t))

    closest_x = x1 + t * dx
    closest_y = y1 + t * dy
    return _distance(px, py, closest_x, closest_y)


def _segment_intersects_circle(x1: float, y1: float, x2: float, y2: float,
                                cx: float, cy: float, radius: float) -> bool:
    """線分(x1,y1)-(x2,y2)が円(cx,cy,radius)と衝突しているかどうかを判定する。"""
    return _point_to_segment_distance(cx, cy, x1, y1, x2, y2) <= radius


def _is_segment_collision_free(
        x1: float, y1: float, x2: float, y2: float,
        obstacles: List[Tuple[float, float, float]]) -> bool:
    """線分(x1,y1)-(x2,y2)が全ての障害物のいずれとも衝突しないかを判定する。"""
    for cx, cy, radius in obstacles:
        if _segment_intersects_circle(x1, y1, x2, y2, cx, cy, radius):
            return False
    return True


def _steer(from_x: float, from_y: float, toward_x: float, toward_y: float,
           step_size: float) -> Tuple[float, float]:
    """fromノードからtoward方向にstep_size分だけ進んだ新しい点を計算する
    （toward自体がstep_size以内にある場合はtoward自体を返す）。
    """
    dist = _distance(from_x, from_y, toward_x, toward_y)
    if dist <= step_size:
        return (toward_x, toward_y)
    ratio = step_size / dist
    return (from_x + (toward_x - from_x) * ratio,
            from_y + (toward_y - from_y) * ratio)


def _find_nearest_node_index(nodes: List[_Node], x: float, y: float) -> int:
    """木の中でサンプル点(x, y)に最も近いノードのインデックスを線形探索で求める。

    学習用途のためナイーブなO(n)実装。本格的な実装ではKD-tree等で高速化する。
    """
    nearest_index = 0
    nearest_distance = math.inf
    for i, node in enumerate(nodes):
        d = _distance(node.x, node.y, x, y)
        if d < nearest_distance:
            nearest_distance = d
            nearest_index = i
    return nearest_index


def _find_neighbor_indices(nodes: List[_Node], x: float, y: float,
                            neighbor_radius: float) -> List[int]:
    """点(x, y)からneighbor_radius以内にある全ノードのインデックスを線形探索で
    求める（親選択とリワイヤで共通して使う近傍集合）。学習用途のためO(n)実装。
    """
    neighbors = []
    for i, node in enumerate(nodes):
        if _distance(node.x, node.y, x, y) <= neighbor_radius:
            neighbors.append(i)
    return neighbors


def _reconstruct_path(nodes: List[_Node], node_index: int) -> List[Tuple[float, float]]:
    """木の中で、goalノードからnode_indexまで親リンクをたどって経路を復元する。"""
    path = []
    current = node_index
    while current != -1:
        path.append((nodes[current].x, nodes[current].y))
        current = nodes[current].parent_index
    # 復元順はgoal->startなので、start->goalの順に反転する
    path.reverse()
    return path


def _update_descendant_costs(nodes: List[_Node], node_index: int) -> None:
    """node_indexを親とする子ノードのコストを再帰的に更新する
    （リワイヤによって親ノードのコストが変わったとき、子孫全体に伝播させるため）。

    学習用途のため、親リンクをたどる代わりに毎回全ノードを線形走査して
    直接の子を探す素朴な実装にしている（O(n)、深い木ではさらに再帰呼び出し分かかる）。
    """
    parent = nodes[node_index]
    for i, node in enumerate(nodes):
        if node.parent_index == node_index:
            node.cost = parent.cost + _distance(parent.x, parent.y, node.x, node.y)
            _update_descendant_costs(nodes, i)


def rrt_star(obstacles: List[Tuple[float, float, float]],
             bounds: Tuple[float, float, float, float],
             start: Tuple[float, float], goal: Tuple[float, float],
             goal_radius: float, step_size: float, max_iterations: int,
             seed: int) -> List[Tuple[float, float]]:
    """RRT*で2次元連続空間上の経路を探索する。

    Args:
        obstacles: 障害物のリスト。各要素は(center_x, center_y, radius)。
        bounds: 探索空間の矩形領域 (xmin, ymin, xmax, ymax)。
        start: 探索開始点(x, y)。
        goal: 目標点(x, y)。
        goal_radius: goalからこの半径以内に到達したら成功とみなす。
        step_size: 1ステップで木を伸ばす距離。
        max_iterations: 最大反復回数。これを超えても到達できなければ空リストを返す。
        seed: 乱数生成器を初期化するシード値（再現性確保のため必須）。

    Returns:
        startからgoal付近までの経路（(x, y)の点のリスト）。
        先頭がstart、末尾がgoal付近の到達点。見つからなければ空リスト。
    """
    xmin, ymin, xmax, ymax = bounds
    rng = random.Random(seed)

    # 親選択・リワイヤで使う近傍半径。シンプルな固定値（step_sizeの定数倍）を
    # 採用する。値が大きいほど最適化の効果は高まるが、近傍探索・リワイヤの
    # コストも増える。
    neighbor_radius = step_size * 5.0

    nodes = [_Node(start[0], start[1], -1, 0.0)]

    for _ in range(max_iterations):
        # 1. 探索空間内から完全にランダムな点をサンプリングする（goalバイアスなし）
        sample_x = rng.uniform(xmin, xmax)
        sample_y = rng.uniform(ymin, ymax)

        # 2. 木の中でサンプル点に最も近いノードを探す（線形探索、O(n)）
        nearest_index = _find_nearest_node_index(nodes, sample_x, sample_y)
        nearest_node = nodes[nearest_index]

        # 3. 最近傍ノードからサンプル点方向にstep_size分だけ伸ばした新しい点を作る
        new_x, new_y = _steer(nearest_node.x, nearest_node.y, sample_x, sample_y,
                               step_size)

        # 4. 新しい点への線分が障害物と衝突していなければ次に進む
        if not _is_segment_collision_free(nearest_node.x, nearest_node.y, new_x,
                                           new_y, obstacles):
            continue

        # 5. 親選択（choose parent）: 新しい点の近傍集合の中から、
        #    「ノードのコスト + そのノードから新しい点までの距離」が最小で、
        #    かつ間の線分が衝突しないノードを親として選ぶ
        #    （ただし採用条件はstep_size以内のノードに限定する）。
        neighbors = _find_neighbor_indices(nodes, new_x, new_y, neighbor_radius)

        best_parent_index = nearest_index
        best_cost = nearest_node.cost + _distance(nearest_node.x, nearest_node.y,
                                                    new_x, new_y)

        for candidate_index in neighbors:
            candidate = nodes[candidate_index]
            candidate_dist = _distance(candidate.x, candidate.y, new_x, new_y)
            if candidate_dist > step_size:
                continue
            candidate_cost = candidate.cost + candidate_dist
            if candidate_cost < best_cost and _is_segment_collision_free(
                    candidate.x, candidate.y, new_x, new_y, obstacles):
                best_cost = candidate_cost
                best_parent_index = candidate_index

        # 6. 新しい点をそのコストとともに木に追加する。
        nodes.append(_Node(new_x, new_y, best_parent_index, best_cost))
        new_index = len(nodes) - 1

        # 7. リワイヤ（rewire）: 新しいノードの近傍集合にある既存の各ノードについて、
        #    新しいノード経由のコストが現在のコストより小さく、かつ衝突しないなら
        #    親を新しいノードに付け替え、コストを更新する（子孫のコストも伝播）。
        #    こちらもstep_size以内のノードのみを対象にする（上記と同じ理由）。
        for neighbor_index in neighbors:
            if neighbor_index == best_parent_index:
                continue
            neighbor = nodes[neighbor_index]
            dist_to_neighbor = _distance(new_x, new_y, neighbor.x, neighbor.y)
            if dist_to_neighbor > step_size:
                continue
            cost_through_new = nodes[new_index].cost + dist_to_neighbor
            if cost_through_new < neighbor.cost and _is_segment_collision_free(
                    new_x, new_y, neighbor.x, neighbor.y, obstacles):
                neighbor.parent_index = new_index
                neighbor.cost = cost_through_new
                _update_descendant_costs(nodes, neighbor_index)

        # 8. 新しいノードがgoalからgoal_radius以内であれば、その時点で経路を
        #    復元して返す。学習用途のため、本実装ではgoal到達時点で即座に
        #    経路を返す。より高度な実装では全イテレーション後に
        #    最良コストの経路を選ぶ。
        if _distance(new_x, new_y, goal[0], goal[1]) <= goal_radius:
            return _reconstruct_path(nodes, new_index)

    # max_iterations以内にgoalへ到達できなかった
    return []
