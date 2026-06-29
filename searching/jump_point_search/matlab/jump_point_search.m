function path = jump_point_search(grid, start, goal)
% JUMP_POINT_SEARCH Jump Point Search (JPS) によるグリッド上の最短経路探索
%
%   グリッド上のA*探索を高速化したアルゴリズム。8方向の均一コストグリッド
%   という前提（直進コスト1、斜めコストsqrt(2)）を利用し、対称性を持つ
%   経路を1セルずつ展開するのではなく、「強制近傍(forced neighbor)」が
%   現れるセルまで一気に飛ばして（ジャンプして）探索する。これにより
%   探索するノード数がA*より大幅に減ることが多い。
%
%   近傍の刈り込み(pruning): 親ノードからの進入方向(dr,dc)に応じて、まず
%   自然近傍（直進方向、斜め進入ならその水平・垂直成分）を候補に入れる。
%   加えて「強制近傍」、すなわち1手前のセルからは到達できなかった方向が
%   現在のセルからは到達できるようになる方向（Harabor & Grastien, 2011の
%   定義をコーナーカット禁止のcanMoveにそのまま適用したもの）を候補に
%   加える。これにより最短経路の最適性を保ったまま探索すべき方向を
%   大幅に絞り込む。コーナーカット許容前提でハードコードされた教科書通り
%   の障害物判定（側方セルが障害物なら強制近傍、等）はコーナーカット禁止
%   下では判定条件と提案される斜め移動が両立しないことがあり、必要な
%   近傍を取り逃すため採用していない。
%
%   ジャンプ(jump)関数: ある方向に直進し続け、(a)障害物または範囲外に
%   達した場合は失敗、(b)ゴールに達した場合は成功、(c)強制近傍を持つ
%   セルに達した場合は成功、を判定する。斜め方向のジャンプでは、各ステップ
%   で水平・垂直方向のサブジャンプも試行し、いずれかが成功すれば現在地点
%   もジャンプポイントとなる。
%
%   移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
%   斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
%   その両側の直交マスのいずれかが障害物の場合は斜め移動できない。
%
%   ヒューリスティック: Octile distance（A*系列の他アルゴリズムと共通）。
%
%   時間複雑度: 最悪ケースはA*と同じO(V log V + E)だが、実用上は
%               ジャンプによって展開ノード数が大幅に減るため高速。
%   空間複雑度: O(V)
%
%   path = JUMP_POINT_SEARCH(grid, start, goal)
%     grid:  2次元配列。0=通行可能, 1=障害物。
%     start: 探索開始地点 [row col]（1始まり座標）。
%     goal:  目的地 [row col]（1始まり座標）。
%
%   戻り値 path は startからgoalまでの最短経路を表すN×2行列
%   （各行が1始まりの[row col]、両端を含み、ジャンプポイント間も1セル
%   ずつ展開した連続経路）。到達不可能な場合は空行列 [] を返す。
%
%   事前条件: start/goalはグリッド範囲内かつ通行可能マスであること
%             （呼び出し側の責務。本関数内では検証しない）。

    [numRows, numCols] = size(grid);
    sqrt2 = sqrt(2);

    if isequal(start, goal)
        path = start;
        return;
    end

    gScore = nan(numRows, numCols);
    gScore(start(1), start(2)) = 0;

    parentRow = zeros(numRows, numCols);
    parentCol = zeros(numRows, numCols);
    hasParent = false(numRows, numCols);

    closed = false(numRows, numCols);

    h0 = octileDistance(start, goal, sqrt2);
    openList = [h0, start(1), start(2)];

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

        if hasParent(row, col)
            parent = [parentRow(row, col), parentCol(row, col)];
        else
            parent = [];
        end
        dirs = prunedDirections(grid, row, col, parent, numRows, numCols);

        for d = 1:size(dirs, 1)
            dr = dirs(d, 1);
            dc = dirs(d, 2);

            jp = jumpPoint(grid, row, col, dr, dc, goal, numRows, numCols);
            if isempty(jp)
                continue;
            end
            jr = jp(1);
            jc = jp(2);

            if closed(jr, jc)
                continue;
            end

            steps = max(abs(jr - row), abs(jc - col));
            if dr ~= 0 && dc ~= 0
                stepCost = sqrt2;
            else
                stepCost = 1;
            end
            tentativeG = currentG + steps * stepCost;

            if isnan(gScore(jr, jc)) || tentativeG < gScore(jr, jc)
                gScore(jr, jc) = tentativeG;
                parentRow(jr, jc) = row;
                parentCol(jr, jc) = col;
                hasParent(jr, jc) = true;
                f = tentativeG + octileDistance([jr jc], goal, sqrt2);
                openList = [openList; f, jr, jc]; %#ok<AGROW>
            end
        end
    end

    if ~found
        path = [];
        return;
    end

    % ジャンプポイントの列をたどって経路を復元する。
    jumpPoints = [goal(1), goal(2)];
    r = goal(1);
    c = goal(2);
    while ~(r == start(1) && c == start(2))
        pr = parentRow(r, c);
        pc = parentCol(r, c);
        r = pr;
        c = pc;
        jumpPoints = [r, c; jumpPoints]; %#ok<AGROW>
    end

    % ジャンプポイント間は直線（直進または斜め45度）でつながっているため、
    % 1セルずつ展開して連続経路を構築する。
    path = jumpPoints(1, :);
    for i = 2:size(jumpPoints, 1)
        fromRow = jumpPoints(i - 1, 1);
        fromCol = jumpPoints(i - 1, 2);
        toRow = jumpPoints(i, 1);
        toCol = jumpPoints(i, 2);
        dr = sign(toRow - fromRow);
        dc = sign(toCol - fromCol);
        r = fromRow;
        c = fromCol;
        while ~(r == toRow && c == toCol)
            r = r + dr;
            c = c + dc;
            path = [path; r, c]; %#ok<AGROW>
        end
    end
end

function h = octileDistance(a, b, sqrt2)
% OCTILEDISTANCE Octile distance（許容的ヒューリスティック）を計算する。
    dx = abs(a(1) - b(1));
    dy = abs(a(2) - b(2));
    h = max(dx, dy) + (sqrt2 - 1) * min(dx, dy);
end

function ok = canMove(grid, row, col, dr, dc, numRows, numCols)
% CANMOVE (row,col)から方向(dr,dc)へ移動できるかを判定する（コーナーカット禁止含む）。
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

function ok = isNaturalDirection(dr, dc, ddr, ddc)
% ISNATURALDIRECTION entry方向(dr,dc)に対する自然近傍かどうかを判定する。
    if dr ~= 0 && dc ~= 0
        ok = (ddr == dr && ddc == 0) || (ddr == 0 && ddc == dc) || (ddr == dr && ddc == dc);
    else
        ok = (ddr == dr && ddc == dc);
    end
end

function dirs = forcedNeighborDirections(grid, row, col, dr, dc, numRows, numCols)
% FORCEDNEIGHBORDIRECTIONS (row,col)に方向(dr,dc)で進入した際の
% 強制近傍方向の一覧を返す。
%
%   Harabor & Grastienによる強制近傍の定義「1手前のセルからは到達できな
%   かった近傍が、現在のセルからは到達できる」を、コーナーカット禁止を
%   含むcanMoveに対してそのまま適用する。後退方向（来た方向の正反対）
%   のみを除外し、その他の7方向は新規性のみで判定する。
    allDirRow = [-1 1 0 0 -1 -1 1 1];
    allDirCol = [0 0 -1 1 -1 1 -1 1];

    prevRow = row - dr;
    prevCol = col - dc;
    prevInBounds = prevRow >= 1 && prevRow <= numRows && prevCol >= 1 && prevCol <= numCols;

    dirs = zeros(0, 2);
    for d = 1:8
        ddr = allDirRow(d);
        ddc = allDirCol(d);
        if isNaturalDirection(dr, dc, ddr, ddc)
            continue;
        end
        if ddr == -dr && ddc == -dc
            continue; % 正反対の後退方向は最短経路に寄与しないため除外
        end
        if ~canMove(grid, row, col, ddr, ddc, numRows, numCols)
            continue;
        end
        prevCouldMove = prevInBounds && canMove(grid, prevRow, prevCol, ddr, ddc, numRows, numCols);
        if ~prevCouldMove
            dirs = [dirs; ddr, ddc]; %#ok<AGROW>
        end
    end
end

function ok = hasForcedNeighbor(grid, row, col, dr, dc, numRows, numCols)
% HASFORCEDNEIGHBOR (row,col)に方向(dr,dc)で進入した際に強制近傍を持つか。
    ok = ~isempty(forcedNeighborDirections(grid, row, col, dr, dc, numRows, numCols));
end

function jp = jumpPoint(grid, row, col, dr, dc, goal, numRows, numCols)
% JUMPPOINT 方向(dr,dc)へ直進し続け、ジャンプポイント（強制近傍を持つ
% セル・ゴール・それ以上進めない行き止まりのセル）を探す。最初の一歩目
% で進めない場合（すぐに障害物・範囲外に当たった場合）のみ空行列を返す。
% 1歩以上進めた後に行き止まりになった場合は、その地点を曲がるための
% ジャンプポイントとして返す（グリッド境界で直角に曲がる必要がある経路を
% 取り逃さないため）。
    if ~canMove(grid, row, col, dr, dc, numRows, numCols)
        jp = [];
        return;
    end

    curRow = row + dr;
    curCol = col + dc;

    while true
        if curRow == goal(1) && curCol == goal(2)
            jp = [curRow, curCol];
            return;
        end

        if hasForcedNeighbor(grid, curRow, curCol, dr, dc, numRows, numCols)
            jp = [curRow, curCol];
            return;
        end

        if dr ~= 0 && dc ~= 0
            % 斜め移動の場合、各ステップで水平・垂直方向のサブジャンプも試す。
            if ~isempty(jumpPoint(grid, curRow, curCol, dr, 0, goal, numRows, numCols))
                jp = [curRow, curCol];
                return;
            end
            if ~isempty(jumpPoint(grid, curRow, curCol, 0, dc, goal, numRows, numCols))
                jp = [curRow, curCol];
                return;
            end
        end

        if ~canMove(grid, curRow, curCol, dr, dc, numRows, numCols)
            jp = [curRow, curCol];
            return;
        end
        curRow = curRow + dr;
        curCol = curCol + dc;
    end
end

function dirs = prunedDirections(grid, row, col, parent, numRows, numCols)
% PRUNEDDIRECTIONS 現在ノードについて、探索すべき方向(dr,dc)の集合を
% 刈り込んで返す。parentが空の場合（start地点）は8方向すべてを候補とする。
    allDirRow = [-1 1 0 0 -1 -1 1 1];
    allDirCol = [0 0 -1 1 -1 1 -1 1];

    if isempty(parent)
        dirs = zeros(0, 2);
        for d = 1:8
            if canMove(grid, row, col, allDirRow(d), allDirCol(d), numRows, numCols)
                dirs = [dirs; allDirRow(d), allDirCol(d)]; %#ok<AGROW>
            end
        end
        return;
    end

    dr = sign(row - parent(1));
    dc = sign(col - parent(2));

    dirs = zeros(0, 2);

    % 自然近傍（直進方向、斜め進入ならその水平・垂直成分も含む）。
    if dr ~= 0 && dc ~= 0
        if canMove(grid, row, col, dr, 0, numRows, numCols)
            dirs = [dirs; dr, 0];
        end
        if canMove(grid, row, col, 0, dc, numRows, numCols)
            dirs = [dirs; 0, dc];
        end
    end
    if canMove(grid, row, col, dr, dc, numRows, numCols)
        dirs = [dirs; dr, dc];
    end

    % 強制近傍（hasForcedNeighborと同じ「1手前では不可、現在は可能」の定義）。
    forced = forcedNeighborDirections(grid, row, col, dr, dc, numRows, numCols);
    if ~isempty(forced)
        dirs = [dirs; forced];
    end
end
