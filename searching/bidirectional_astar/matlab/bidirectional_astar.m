function path = bidirectional_astar(grid, start, goal)
% BIDIRECTIONAL_ASTAR 双方向A*探索（Bidirectional A* search）
%
%   startからgoalへの通常のA*探索（前進探索）と、goalからstartへの
%   A*探索（後退探索）を同時に走らせ、2つの探索の確定済み（closed）集合が
%   共通のノードを持った時点で、両方向の最短経路を結合して全体の最短経路を
%   構築するアルゴリズム。各方向のヒューリスティックにはOctile distanceを
%   使用し、前進探索は相手側のgoal、後退探索は相手側のstartを目標とする。
%
%   終了条件: 両側の優先度付きキューを交互に1ステップずつ展開し、ある
%   ノードが両方のclosed集合に含まれた瞬間、それを「交差点(meeting node)」
%   とみなす。その時点で両側のキューに残る最小f値の合計がこれまでに見つ
%   かった最良の合計コストを超えないことを確認してから探索を終了することで、
%   最適解を保証する（標準的な双方向探索の停止条件）。
%
%   移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
%   斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
%   その両側の直交マスのいずれかが障害物の場合は斜め移動できない。
%
%   時間複雑度: O(V log V + E)（V=セル数、E=辺数）
%   空間複雑度: O(V)（両方向の探索状態を保持するため定数倍は大きくなる）
%
%   path = BIDIRECTIONAL_ASTAR(grid, start, goal)
%     grid:  2次元配列。0=通行可能, 1=障害物。
%     start: 探索開始地点 [row col]（1始まり座標）。
%     goal:  目的地 [row col]（1始まり座標）。
%
%   戻り値 path は startからgoalまでの最短経路を表すN×2行列
%   （各行が1始まりの[row col]、両端を含む）。到達不可能な場合は
%   空行列 [] を返す。
%
%   事前条件: start/goalはグリッド範囲内かつ通行可能マスであること
%             （呼び出し側の責務。本関数内では検証しない）。

    [numRows, numCols] = size(grid);
    sqrt2 = sqrt(2);

    if isequal(start, goal)
        path = start;
        return;
    end

    startKey = sub2ind([numRows, numCols], start(1), start(2));
    goalKey = sub2ind([numRows, numCols], goal(1), goal(2));

    % 前進探索（start->goal方向）と後退探索（goal->start方向）の状態を
    % それぞれ構造体で保持する。
    forward = initSearchState(start, goal, numRows, numCols, sqrt2);
    backward = initSearchState(goal, start, numRows, numCols, sqrt2);

    bestCost = inf;
    meetingKey = -1;

    % 交互に1ノードずつ展開する。あるノードが両方のgScoreに含まれた時点で
    % その経路長を候補とし、両側のオープンリストの最小f値の合計が候補
    % コスト以上になった時点で最適性が保証されるため探索を終了する。
    while ~isempty(forward.openList) && ~isempty(backward.openList)
        forwardTopF = min(forward.openList(:, 1));
        backwardTopF = min(backward.openList(:, 1));
        if forwardTopF + backwardTopF >= bestCost
            break;
        end

        [forward, expandedForwardKey] = expandOne(forward, grid, numRows, numCols, sqrt2);
        if expandedForwardKey ~= -1 && ~isnan(backward.gScore(expandedForwardKey))
            candidate = forward.gScore(expandedForwardKey) + backward.gScore(expandedForwardKey);
            if candidate < bestCost
                bestCost = candidate;
                meetingKey = expandedForwardKey;
            end
        end

        if isempty(forward.openList) || isempty(backward.openList)
            break;
        end

        [backward, expandedBackwardKey] = expandOne(backward, grid, numRows, numCols, sqrt2);
        if expandedBackwardKey ~= -1 && ~isnan(forward.gScore(expandedBackwardKey))
            candidate = forward.gScore(expandedBackwardKey) + backward.gScore(expandedBackwardKey);
            if candidate < bestCost
                bestCost = candidate;
                meetingKey = expandedBackwardKey;
            end
        end
    end

    if meetingKey == -1
        path = []; % 到達不可能
        return;
    end

    % 前進側: start -> meetingKey
    [meetRow, meetCol] = ind2sub([numRows, numCols], meetingKey);
    forwardPath = [meetRow, meetCol];
    currentKey = meetingKey;
    while currentKey ~= startKey
        currentKey = forward.parent(currentKey);
        [r, c] = ind2sub([numRows, numCols], currentKey);
        forwardPath = [r, c; forwardPath]; %#ok<AGROW>
    end

    % 後退側: meetingKey -> goal（backward.parentはgoal方向への木なので
    % meetingKeyから辿るとgoalに到達する順序になる）
    backwardPath = [];
    currentKey = meetingKey;
    while currentKey ~= goalKey
        currentKey = backward.parent(currentKey);
        [r, c] = ind2sub([numRows, numCols], currentKey);
        backwardPath = [backwardPath; r, c]; %#ok<AGROW>
    end

    path = [forwardPath; backwardPath];
end

function state = initSearchState(source, target, numRows, numCols, sqrt2)
% INITSEARCHSTATE 一方向分の探索状態を初期化する（前進・後退どちらにも使う）。
    sourceKey = sub2ind([numRows, numCols], source(1), source(2));

    state.gScore = nan(numRows * numCols, 1);
    state.gScore(sourceKey) = 0;
    state.parent = zeros(numRows * numCols, 1);
    state.closed = false(numRows * numCols, 1);
    state.target = target;

    h0 = octileDistance(source, target, sqrt2);
    state.openList = [h0, sourceKey];
end

function [state, expandedKey] = expandOne(state, grid, numRows, numCols, sqrt2)
% EXPANDONE stateを1ステップ進める（オープンリストの最小f値を1つ展開する）。
%   展開したノードのキーをexpandedKeyに格納する。展開できなければ-1を返す。
    dirs = [-1 0; 1 0; 0 -1; 0 1; -1 -1; -1 1; 1 -1; 1 1];
    expandedKey = -1;

    while ~isempty(state.openList)
        [~, idx] = min(state.openList(:, 1));
        currentKey = state.openList(idx, 2);
        state.openList(idx, :) = [];

        if state.closed(currentKey)
            continue; % 古いエントリはスキップ
        end
        state.closed(currentKey) = true;
        expandedKey = currentKey;

        [row, col] = ind2sub([numRows, numCols], currentKey);
        currentG = state.gScore(currentKey);

        for d = 1:size(dirs, 1)
            dr = dirs(d, 1);
            dc = dirs(d, 2);

            if ~canMove(grid, row, col, dr, dc, numRows, numCols)
                continue;
            end

            newRow = row + dr;
            newCol = col + dc;
            neighborKey = sub2ind([numRows, numCols], newRow, newCol);

            if state.closed(neighborKey)
                continue;
            end

            if dr ~= 0 && dc ~= 0
                stepCost = sqrt2;
            else
                stepCost = 1;
            end
            tentativeG = currentG + stepCost;

            if isnan(state.gScore(neighborKey)) || tentativeG < state.gScore(neighborKey)
                state.gScore(neighborKey) = tentativeG;
                state.parent(neighborKey) = currentKey;
                f = tentativeG + octileDistance([newRow newCol], state.target, sqrt2);
                state.openList = [state.openList; f, neighborKey]; %#ok<AGROW>
            end
        end

        return; % 1ノード展開した
    end
end

function h = octileDistance(a, b, sqrt2)
% OCTILEDISTANCE Octile distance（許容的ヒューリスティック）を計算する。
    dx = abs(a(1) - b(1));
    dy = abs(a(2) - b(2));
    h = max(dx, dy) + (sqrt2 - 1) * min(dx, dy);
end

function ok = canMove(grid, row, col, dr, dc, numRows, numCols)
% CANMOVE (row,col)から方向(dr,dc)へ移動できるかを判定する。
%   斜め移動の場合はコーナーカット禁止のルールを適用する：
%   両側の直交マスのいずれかが障害物または範囲外であれば移動できない。
    newRow = row + dr;
    newCol = col + dc;

    if newRow < 1 || newRow > numRows || newCol < 1 || newCol > numCols
        ok = false;
        return;
    end
    if grid(newRow, newCol) ~= 0
        ok = false;
        return;
    end

    if dr ~= 0 && dc ~= 0
        if row + dr < 1 || row + dr > numRows || grid(row + dr, col) ~= 0
            ok = false;
            return;
        end
        if col + dc < 1 || col + dc > numCols || grid(row, col + dc) ~= 0
            ok = false;
            return;
        end
    end

    ok = true;
end
