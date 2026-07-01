function path = weighted_astar(grid, start, goal, weight)
% WEIGHTED_ASTAR 重み付きA*探索によるグリッド上の経路探索
%
%   通常のA*は f(n) = g(n) + h(n) でノードを評価するが、重み付きA*では
%   f(n) = g(n) + weight * h(n) としてヒューリスティックの影響を強める。
%   weight > 1.0 にするとヒューリスティックがより強く効くため、ゴール方向への
%   探索が加速し展開ノード数が減って高速になるが、ヒューリスティックが
%   許容的でなくなる（実コストを過大評価しうる）ため最適性は保証されない。
%   weight = 1.0 のときは通常のA*と等価になる。
%
%   本実装ではヒューリスティックとして Octile distance を用いる。
%       dx = |r1 - r2|, dy = |c1 - c2|
%       h = max(dx, dy) + (sqrt(2) - 1) * min(dx, dy)
%
%   移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
%   斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
%   その両側の直交マスのいずれかが障害物の場合は斜め移動できない。
%
%   時間複雑度: O(V log V + E)（V=セル数、E=辺数）
%   空間複雑度: O(V)
%
%   path = WEIGHTED_ASTAR(grid, start, goal, weight)
%     grid:   2次元配列。0=通行可能, 1=障害物。
%     start:  探索開始地点 [row col]（1始まり座標）。
%     goal:   目的地 [row col]（1始まり座標）。
%     weight: ヒューリスティックに掛ける重み（>1.0で高速化、最適性は保証されない）。
%
%   戻り値 path は startからgoalまでの経路を表すN×2行列
%   （各行が1始まりの[row col]、両端を含む）。到達不可能な場合は
%   空行列 [] を返す。
%
%   事前条件: start/goalはグリッド範囲内かつ通行可能マスであること
%             （呼び出し側の責務。本関数内では検証しない）。

    [numRows, numCols] = size(grid);

    dirs = [-1 0; 1 0; 0 -1; 0 1; -1 -1; -1 1; 1 -1; 1 1];
    sqrt2 = sqrt(2);

    if isequal(start, goal)
        path = start;
        return;
    end

    gScore = nan(numRows, numCols);
    gScore(start(1), start(2)) = 0;

    parentRow = zeros(numRows, numCols);
    parentCol = zeros(numRows, numCols);

    closed = false(numRows, numCols);

    h0 = octileDistance(start, goal, sqrt2);
    openList = [weight * h0, start(1), start(2)];

    found = false;
    while ~isempty(openList)
        [~, idx] = min(openList(:, 1));
        current = openList(idx, :);
        openList(idx, :) = [];

        row = current(2);
        col = current(3);

        if closed(row, col)
            continue;
        end
        closed(row, col) = true;

        if row == goal(1) && col == goal(2)
            found = true;
            break;
        end

        currentG = gScore(row, col);

        for d = 1:size(dirs, 1)
            dr = dirs(d, 1);
            dc = dirs(d, 2);

            if ~canMove(grid, row, col, dr, dc, numRows, numCols)
                continue;
            end

            newRow = row + dr;
            newCol = col + dc;

            if closed(newRow, newCol)
                continue;
            end

            if dr ~= 0 && dc ~= 0
                stepCost = sqrt2;
            else
                stepCost = 1;
            end
            tentativeG = currentG + stepCost;

            if isnan(gScore(newRow, newCol)) || tentativeG < gScore(newRow, newCol)
                gScore(newRow, newCol) = tentativeG;
                parentRow(newRow, newCol) = row;
                parentCol(newRow, newCol) = col;
                f = tentativeG + weight * octileDistance([newRow newCol], goal, sqrt2);
                openList = [openList; f, newRow, newCol]; %#ok<AGROW>
            end
        end
    end

    if ~found
        path = [];
        return;
    end

    path = [goal(1), goal(2)];
    r = goal(1);
    c = goal(2);
    while ~(r == start(1) && c == start(2))
        pr = parentRow(r, c);
        pc = parentCol(r, c);
        r = pr;
        c = pc;
        path = [r, c; path]; %#ok<AGROW>
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
%   斜め移動の場合はコーナーカット禁止のルールを適用する。
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
