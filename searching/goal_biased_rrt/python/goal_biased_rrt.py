"""Goal-biased RRT（ゴールバイアス付き高速探索ランダム木）の実装。

基本のRRTと同様に、2次元連続空間上でstartを根とする木をランダムサンプリング
によって段階的に広げていき、goal付近まで到達する経路を探索するサンプリング
ベースの経路計画アルゴリズム。各反復で以下を行う:
  1. 乱数を1つ生成し、それがGOAL_BIAS_PROBABILITY未満であればサンプル点として
     goal座標そのものを使う（ゴールバイアス）。それ以外の場合
     （1 - GOAL_BIAS_PROBABILITY の確率）は探索空間内に完全ランダムな点を
     サンプリングする。これが基本のRRTとの唯一の違いである。
  2. 木の中でサンプル点に最も近いノード（最近傍ノード）を見つける。
  3. 最近傍ノードからサンプル点方向にstep_size分だけ進んだ新しいノードを作る
     （これを「ステア」と呼ぶ）。
  4. 最近傍ノードから新しいノードへの線分が障害物と衝突しなければ、
     新しいノードを木に追加する。
  5. 新しいノードがgoalからgoal_radius以内であれば探索を終了し、
     startから新しいノードまでの経路を木の親リンクをたどって復元する。

一定確率でgoal自体をサンプル点として使うことで、木がgoal方向へ伸びやすくなり、
障害物が少ない/単純なシナリオでは基本のRRTより少ない反復回数で収束する傾向が
ある（ただしgoalバイアスが強すぎると局所的な障害物にトラップされやすくなる
トレードオフがあるため、確率は小さめの値、例えば10%程度に設定する）。

時間・空間複雑度:
  最近傍探索は、学習用途のため木の全ノードを線形走査するナイーブな実装
  （O(n)、nは現在の木のノード数）になっている。本格的な実装ではKD-tree等の
  空間分割構造を使って最近傍探索をO(log n)程度に高速化する。
  1イテレーションあたりO(n)の最近傍探索を行うため、合計でO(n^2)程度
  （n = max_iterations、最悪ケース）。空間複雑度はノードを保持するためO(n)。
"""
import math
import random
from typing import List, Tuple

# goalをサンプル点として選ぶ確率（ゴールバイアス）。残り(1 - GOAL_BIAS_PROBABILITY)
# の確率では探索空間内に完全ランダムな点をサンプリングする。
GOAL_BIAS_PROBABILITY = 0.1


class _Node:
    """木の1ノード。parent_indexで親ノードへのリンクを保持する。"""

    __slots__ = ("x", "y", "parent_index")

    def __init__(self, x: float, y: float, parent_index: int):
        self.x = x
        self.y = y
        self.parent_index = parent_index  # startノードは-1


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


def goal_biased_rrt(obstacles: List[Tuple[float, float, float]],
                     bounds: Tuple[float, float, float, float],
                     start: Tuple[float, float], goal: Tuple[float, float],
                     goal_radius: float, step_size: float, max_iterations: int,
                     seed: int) -> List[Tuple[float, float]]:
    """Goal-biased RRTで2次元連続空間上の経路を探索する。

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

    nodes = [_Node(start[0], start[1], -1)]

    for _ in range(max_iterations):
        # 1. GOAL_BIAS_PROBABILITYの確率でgoal自体をサンプル点として使う
        #    （ゴールバイアス）。それ以外は探索空間内に完全ランダムな点を
        #    サンプリングする。
        if rng.random() < GOAL_BIAS_PROBABILITY:
            sample_x, sample_y = goal[0], goal[1]
        else:
            sample_x = rng.uniform(xmin, xmax)
            sample_y = rng.uniform(ymin, ymax)

        # 2. 木の中でサンプル点に最も近いノードを探す（線形探索、O(n)）
        nearest_index = _find_nearest_node_index(nodes, sample_x, sample_y)
        nearest_node = nodes[nearest_index]

        # 3. 最近傍ノードからサンプル点方向にstep_size分だけ伸ばした新しい点を作る
        new_x, new_y = _steer(nearest_node.x, nearest_node.y, sample_x, sample_y,
                               step_size)

        # 4. 新しい点への線分が障害物と衝突していなければ木に追加する
        if not _is_segment_collision_free(nearest_node.x, nearest_node.y, new_x,
                                           new_y, obstacles):
            continue

        nodes.append(_Node(new_x, new_y, nearest_index))
        new_index = len(nodes) - 1

        # 5. 新しいノードがgoalからgoal_radius以内であれば経路を復元して返す
        if _distance(new_x, new_y, goal[0], goal[1]) <= goal_radius:
            return _reconstruct_path(nodes, new_index)

    # max_iterations以内にgoalへ到達できなかった
    return []
