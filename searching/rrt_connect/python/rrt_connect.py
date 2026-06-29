"""RRT-Connect（双方向RRT / Bidirectional Rapidly-exploring Random Tree）の実装。

startを根とする木（treeA）とgoalを根とする木（treeB）の2本を同時に育て、
互いに向かって伸ばし合うことで通常のRRTより高速にstart-goal間の経路を見つける
サンプリングベースの経路計画アルゴリズム。各反復で以下を行う:
  1. 探索空間内からランダムに点（サンプル点）を1つ選ぶ（goalバイアスなし）。
  2. treeAに対して「extend」処理を行う: treeA内の最近傍ノードを見つけ、
     サンプル点方向にstep_size分ステアし、衝突しなければ新しいノードを
     treeAに追加する（基本RRTの1ステップと同じ処理）。
  3. extendが成功したら、その新しいノードに向けてtreeBから「connect」を
     試みる: treeB内の最近傍ノードから新しいノードに向かって、衝突しない
     限りstep_size刻みで繰り返しステアし続ける（targetに到達するか、
     衝突するか、ステア回数の上限に達するまで）。これによりtreeBがtreeAの
     新しいノードに「届く」可能性がある。
  4. 2つの木のノードがgoal_radius以内（またはstep_size以内）まで近づいたら
     接続成功とみなし、treeA側の経路（root→接続点）とtreeB側の経路
     （接続点→root）を結合してstart→goalの完全な経路を構築する。
     treeBはgoalを根にしているため、treeBの経路は反転させてから連結する。
  5. 次のイテレーションではtreeAとtreeBの役割を交換し、両方の木が均等に
     成長するようにする。
  6. max_iterations以内に接続できなければ空リストを返す。

厳密なRRT-Connect論文のtrapped/advanced状態遷移は再現していない簡略版だが、
「両方の木が交互に伸び、互いに接続を試み、接続できたら経路を結合する」という
核心的な振る舞いは実装している。connectのステア繰り返し回数には上限を設けて
無限ループを避けている。

時間・空間複雑度:
  最近傍探索は、学習用途のため木の全ノードを線形走査するナイーブな実装
  （O(n)、nは木のノード数）になっている。本格的な実装ではKD-tree等の
  空間分割構造を使って最近傍探索をO(log n)程度に高速化する。
  1イテレーションではextendで最近傍探索1回（O(n)）、connectで複数回の
  ステア・最近傍探索（定数倍はconnectの繰り返し回数の上限）を行うため
  1イテレーションあたりO(n)程度。合計でO(n^2)程度
  （n = max_iterations、最悪ケース）。空間複雑度は2本の木のノードを
  保持するためO(n)。
"""
import math
import random
from typing import List, Optional, Tuple


class _Node:
    """木の1ノード。parent_indexで親ノードへのリンクを保持する。"""

    __slots__ = ("x", "y", "parent_index")

    def __init__(self, x: float, y: float, parent_index: int):
        self.x = x
        self.y = y
        self.parent_index = parent_index  # 根ノードは-1


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


def _path_to_root(nodes: List[_Node], node_index: int) -> List[Tuple[float, float]]:
    """木の中で、node_indexから親リンクをたどって根までの経路を復元する
    （根→node_indexの順ではなく、node_index→根の順で返す。呼び出し側で
     必要に応じて反転する）。
    """
    path = []
    current = node_index
    while current != -1:
        path.append((nodes[current].x, nodes[current].y))
        current = nodes[current].parent_index
    return path


def _extend(tree: List[_Node], sample_x: float, sample_y: float,
            step_size: float,
            obstacles: List[Tuple[float, float, float]]) -> Optional[int]:
    """extend処理: treeに対して、サンプル点(sample_x, sample_y)方向へ1ステップ
    だけ伸ばす。最近傍ノードを探し、ステアし、衝突しなければ新しいノードを
    treeに追加する。追加した場合は新ノードのインデックスを返し、衝突した
    場合はNoneを返す（基本RRTの1反復と同じ処理）。
    """
    nearest_index = _find_nearest_node_index(tree, sample_x, sample_y)
    nearest_node = tree[nearest_index]

    new_x, new_y = _steer(nearest_node.x, nearest_node.y, sample_x, sample_y,
                           step_size)

    if not _is_segment_collision_free(nearest_node.x, nearest_node.y, new_x,
                                       new_y, obstacles):
        return None

    tree.append(_Node(new_x, new_y, nearest_index))
    return len(tree) - 1


def _connect(tree: List[_Node], target_x: float, target_y: float,
             step_size: float, goal_radius: float, max_steer_steps: int,
             obstacles: List[Tuple[float, float, float]]) -> Tuple[int, bool]:
    """connect処理: treeの最近傍ノードから、target_x, target_yに向かって、
    衝突しない限りstep_size刻みで繰り返しステアし続ける。targetに到達するか、
    衝突するか、ステア回数の上限に達するまで継続する。

    Returns:
        (last_added_index, reached_target)のタプル。
        last_added_indexは最終的にtreeに追加された最後のノードのインデックス
        （1つも追加できなかった場合は-1）。reached_targetはtargetまで
        goal_radius以内に到達できたかどうか。
    """
    reached_target = False
    last_added_index = -1

    for _ in range(max_steer_steps):
        new_index = _extend(tree, target_x, target_y, step_size, obstacles)
        if new_index is None:
            # 衝突して伸ばせなかった（trapped）。connectを中断する。
            break
        last_added_index = new_index

        d = _distance(tree[new_index].x, tree[new_index].y, target_x, target_y)
        if d <= goal_radius:
            reached_target = True
            break
        # d <= step_sizeの場合、次のステアでtarget自体に到達する想定。
        # そのままループを継続してもう一度ステアする。

    return last_added_index, reached_target


def rrt_connect(obstacles: List[Tuple[float, float, float]],
                 bounds: Tuple[float, float, float, float],
                 start: Tuple[float, float], goal: Tuple[float, float],
                 goal_radius: float, step_size: float, max_iterations: int,
                 seed: int) -> List[Tuple[float, float]]:
    """RRT-Connectで2次元連続空間上の経路を探索する。

    Args:
        obstacles: 障害物のリスト。各要素は(center_x, center_y, radius)。
        bounds: 探索空間の矩形領域 (xmin, ymin, xmax, ymax)。
        start: 探索開始点(x, y)。
        goal: 目標点(x, y)。
        goal_radius: 2つの木のノードがこの半径以内まで近づいたら接続成功とみなす。
        step_size: 1ステップで木を伸ばす距離。
        max_iterations: 最大反復回数。これを超えても接続できなければ空リストを返す。
        seed: 乱数生成器（random.Random）を初期化するシード値（再現性確保のため必須）。

    Returns:
        startからgoal付近までの経路（(x, y)の点のリスト）。
        先頭がstart、末尾がgoal付近の到達点。見つからなければ空リスト。
    """
    xmin, ymin, xmax, ymax = bounds
    rng = random.Random(seed)

    # connectでのステア繰り返し回数の上限。探索空間の対角線長をstep_sizeで
    # 割った値（往復しても十分な余裕を持たせるため2倍）を基準にする。
    diagonal = _distance(xmin, ymin, xmax, ymax)
    max_steer_steps = max(100, int(2.0 * diagonal / step_size) + 10)

    # tree_a = startを根とする木、tree_b = goalを根とする木
    tree_a = [_Node(start[0], start[1], -1)]
    tree_b = [_Node(goal[0], goal[1], -1)]

    # swappedがTrueのとき、tree_aが実際にはgoal側の木である
    # （経路結合時にどちらをstart側として扱うか判定するために使う）
    swapped = False

    for _ in range(max_iterations):
        # 1. 探索空間内から完全にランダムな点をサンプリングする（goalバイアスなし）
        sample_x = rng.uniform(xmin, xmax)
        sample_y = rng.uniform(ymin, ymax)

        # 2. tree_aに対してextend処理を行う（基本RRTの1ステップと同じ）
        new_index_a = _extend(tree_a, sample_x, sample_y, step_size, obstacles)
        if new_index_a is not None:
            # 3. extendが成功したので、tree_bから新しいノードへconnectを試みる
            target_x = tree_a[new_index_a].x
            target_y = tree_a[new_index_a].y

            new_index_b, reached = _connect(tree_b, target_x, target_y,
                                              step_size, goal_radius,
                                              max_steer_steps, obstacles)

            if reached and new_index_b != -1:
                # 4. 2つの木が接続できた。tree_a側の経路とtree_b側の経路を結合する。
                # _path_to_root(tree, idx)はidx→...→rootの順を返す。
                # tree_aの経路（idx=new_index_a→...→tree_aのroot）
                path_a = _path_to_root(tree_a, new_index_a)
                # tree_bの経路（idx=new_index_b→...→tree_bのroot）
                path_b = _path_to_root(tree_b, new_index_b)

                # start→...→goalの順になるよう、それぞれの根がどちらかに
                # 応じて反転するかどうかを決める。
                if not swapped:
                    # tree_a=start側（root=start）、tree_b=goal側（root=goal）
                    # path_a: new_index_a→...→start なので反転してstart→new_index_aに、
                    # path_b: new_index_b→...→goal はそのまま末尾に連結する。
                    path_a.reverse()
                    full_path = path_a + path_b
                else:
                    # tree_a=goal側（root=goal）、tree_b=start側（root=start）
                    # path_b: new_index_b→...→start なので反転してstart→new_index_bに、
                    # path_a: new_index_a→...→goal はそのまま末尾に連結する。
                    path_b.reverse()
                    full_path = path_b + path_a
                return full_path

        # 6. 次のイテレーションのためにtree_aとtree_bの役割を交換する
        tree_a, tree_b = tree_b, tree_a
        swapped = not swapped

    # max_iterations以内に接続できなかった
    return []
