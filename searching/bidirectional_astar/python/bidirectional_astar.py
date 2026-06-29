"""双方向A*探索（Bidirectional A* search）の実装。

startからgoalへの通常のA*探索（前進探索）と、goalからstartへの
A*探索（後退探索）を同時に走らせ、2つの探索の確定済み（closed）集合が
共通のノードを持った時点で、両方向の最短経路を結合して全体の最短経路を
構築するアルゴリズム。各方向のヒューリスティックにはOctile distanceを
使用し、前進探索は相手側のgoal、後退探索は相手側のstartを目標とする。

終了条件: 両側の優先度付きキューを交互に1ステップずつ展開し、ある
ノードが両方のclosed集合に含まれた瞬間、それを「交差点(meeting node)」
とみなす。その時点で両側の優先度付きキューに残る最小f値の合計が
これまでに見つかった最良の合計コストを超えないことを確認してから
探索を終了することで、最適解を保証する（標準的な双方向探索の停止条件）。

移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
その両側の直交マスのいずれかが障害物の場合は斜め移動できない。

時間複雑度: O(V log V + E)（V=セル数、E=辺数。理論上は単方向A*よりも
            探索範囲が小さくなることが多く実用上高速になる傾向がある）
空間複雑度: O(V)（両方向の探索状態を保持するため定数倍は大きくなる）
"""
import heapq
import math
from typing import Dict, List, Optional, Tuple

_DIRECTIONS = [(-1, 0), (1, 0), (0, -1), (0, 1), (-1, -1), (-1, 1), (1, -1), (1, 1)]

_SQRT2 = math.sqrt(2)
_SQRT2_MINUS_1 = _SQRT2 - 1.0


def _octile_distance(a: Tuple[int, int], b: Tuple[int, int]) -> float:
    """Octile distance（許容的ヒューリスティック）を計算する。"""
    dx = abs(a[0] - b[0])
    dy = abs(a[1] - b[1])
    return max(dx, dy) + _SQRT2_MINUS_1 * min(dx, dy)


def _in_bounds(grid: List[List[int]], row: int, col: int) -> bool:
    return 0 <= row < len(grid) and 0 <= col < len(grid[0])


def _is_walkable(grid: List[List[int]], row: int, col: int) -> bool:
    return grid[row][col] == 0


def _can_move(grid: List[List[int]], row: int, col: int, dr: int, dc: int) -> bool:
    """(row,col)から方向(dr,dc)へ移動できるかを判定する（コーナーカット禁止含む）。

    8方向移動は対称（A->Bが可能ならB->Aも可能）なので、前進・後退の両探索で
    同じ関数をそのまま利用できる。
    """
    new_row, new_col = row + dr, col + dc
    if not _in_bounds(grid, new_row, new_col) or not _is_walkable(grid, new_row, new_col):
        return False

    if dr != 0 and dc != 0:
        if not _in_bounds(grid, row + dr, col) or not _is_walkable(grid, row + dr, col):
            return False
        if not _in_bounds(grid, row, col + dc) or not _is_walkable(grid, row, col + dc):
            return False

    return True


def _step_cost(dr: int, dc: int) -> float:
    return _SQRT2 if dr != 0 and dc != 0 else 1.0


class _SearchState:
    """一方向分の探索状態（前進探索・後退探索どちらにも使う共通構造）。"""

    def __init__(self, source: Tuple[int, int], target: Tuple[int, int]) -> None:
        self.g_score: Dict[Tuple[int, int], float] = {source: 0.0}
        self.parent: Dict[Tuple[int, int], Tuple[int, int]] = {}
        self.closed: Dict[Tuple[int, int], bool] = {}
        self.target = target
        self.open_heap: List[Tuple[float, Tuple[int, int]]] = [
            (_octile_distance(source, target), source)
        ]

    def top_f(self) -> float:
        return self.open_heap[0][0]


def _expand_one(
    state: _SearchState, grid: List[List[int]]
) -> Optional[Tuple[int, int]]:
    """stateを1ステップ進める（キューの先頭を1つ展開する）。

    展開が行われればそのノードのキーを返し、キューが空（これ以上展開
    できない）ならNoneを返す。
    """
    while state.open_heap:
        _, current = heapq.heappop(state.open_heap)

        if state.closed.get(current, False):
            continue  # 古いエントリはスキップ
        state.closed[current] = True

        row, col = current
        current_g = state.g_score[current]

        for dr, dc in _DIRECTIONS:
            if not _can_move(grid, row, col, dr, dc):
                continue

            neighbor = (row + dr, col + dc)
            if state.closed.get(neighbor, False):
                continue

            tentative_g = current_g + _step_cost(dr, dc)
            if neighbor not in state.g_score or tentative_g < state.g_score[neighbor]:
                state.g_score[neighbor] = tentative_g
                state.parent[neighbor] = current
                f = tentative_g + _octile_distance(neighbor, state.target)
                heapq.heappush(state.open_heap, (f, neighbor))

        return current  # 1ノード展開した

    return None  # キューが空 = これ以上展開できない


def bidirectional_astar(
    grid: List[List[int]], start: Tuple[int, int], goal: Tuple[int, int]
) -> List[Tuple[int, int]]:
    """startからgoalまでの最短経路を双方向A*で求める。

    Args:
        grid: 2次元配列。0=通行可能, 1=障害物。
        start: 探索開始地点 (row, col)。
        goal: 目的地 (row, col)。

    Returns:
        startからgoalまでの最短経路（両端を含む）を(row,col)のリストで返す。
        到達不可能な場合は空リストを返す。

    事前条件: start/goalはグリッド範囲内かつ通行可能マスであること
              （呼び出し側の責務。本関数内では検証しない）。
    """
    if not grid or not grid[0]:
        return []

    if start == goal:
        return [start]

    forward = _SearchState(start, goal)  # start -> goal方向
    backward = _SearchState(goal, start)  # goal -> start方向

    best_cost = math.inf
    meeting_node: Optional[Tuple[int, int]] = None

    # 交互に1ノードずつ展開する。あるノードが両方のclosed集合に含まれた
    # 時点でその経路長を候補とし、両側のキューの最小f値の合計が候補コスト
    # 以上になった時点で最適性が保証されるため探索を終了する。
    while forward.open_heap and backward.open_heap:
        # 標準的な停止条件: 両側の最小f値の合計が現在の最良コスト以上なら終了。
        if forward.top_f() + backward.top_f() >= best_cost:
            break

        expanded_forward = _expand_one(forward, grid)
        if expanded_forward is not None and expanded_forward in backward.g_score:
            candidate = forward.g_score[expanded_forward] + backward.g_score[expanded_forward]
            if candidate < best_cost:
                best_cost = candidate
                meeting_node = expanded_forward

        if not forward.open_heap or not backward.open_heap:
            break

        expanded_backward = _expand_one(backward, grid)
        if expanded_backward is not None and expanded_backward in forward.g_score:
            candidate = forward.g_score[expanded_backward] + backward.g_score[expanded_backward]
            if candidate < best_cost:
                best_cost = candidate
                meeting_node = expanded_backward

    if meeting_node is None:
        return []  # 到達不可能

    # 前進側: start -> meeting_node
    forward_path = [meeting_node]
    current = meeting_node
    while current != start:
        current = forward.parent[current]
        forward_path.append(current)
    forward_path.reverse()

    # 後退側: meeting_node -> goal（backward.parentはgoal方向への木なので
    # meeting_nodeから辿るとgoalに到達する順序になる）
    backward_path = []
    current = meeting_node
    while current != goal:
        current = backward.parent[current]
        backward_path.append(current)

    return forward_path + backward_path
