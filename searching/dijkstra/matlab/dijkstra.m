function path = dijkstra(grid, start, goal)
% DIJKSTRA ダイクストラ法によるグリッド上の最短経路探索
%
%   グリッド上の単一始点最短経路を求めるアルゴリズム。スタート地点からの
%   実コスト g(n) が最小のノードから順に確定（=優先度キューから取り出して
%   展開）していくことで、各ノードへの最短コストを単調に確定させていく。
%   ヒューリスティックを使わずに全方向へ均等にコストを伝播するため、
%   負の辺重みが無い限り常に最適解（最短経路）を返す。
%
%   移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
%   斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
%   その両側の直交マスのいずれかが障害物の場合は斜め移動できない。
%
%   時間複雑度: O(V log V + E)（V=セル数、E=辺数）
%   空間複雑度: O(V)
%
%   path = DIJKSTRA(grid, start, goal)
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

    dirs = [-1 0; 1 0; 0 -1; 0 1; -1 -1; -1 1; 1 -1; 1 1];
    sqrt2 = sqrt(2);

    if isequal(start, goal)
        path = start;
        return;
    end

    dist = inf(numRows, numCols);
    dist(start(1), start(2)) = 0;

    parentRow = zeros(numRows, numCols);
    parentCol = zeros(numRows, numCols);

    closed = false(numRows, numCols);

    % openList: 各行が [dist, row, col]
    openList = [0, start(1), start(2)];

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

        currentDist = dist(row, col);

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
            tentativeDist = currentDist + stepCost;

            if tentativeDist < dist(newRow, newCol)
                dist(newRow, newCol) = tentativeDist;
                parentRow(newRow, newCol) = row;
                parentCol(newRow, newCol) = col;
                openList = [openList; tentativeDist, newRow, newCol]; %#ok<AGROW>
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
