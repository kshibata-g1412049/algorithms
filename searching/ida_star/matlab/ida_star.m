function path = ida_star(grid, start, goal)
% IDA_STAR IDA*探索（Iterative Deepening A*）によるグリッド上の最短経路探索
%
%   A*の最適性を保ちつつ、open/closedリストのような大きな補助データ構造を
%   使わずに反復深化（iterative deepening）の考え方で経路を探索するアルゴ
%   リズム。各反復では「コスト上限値(bound)」を1つ決め、f(n) = g(n) + h(n)
%   がその上限を超えるノードを枝刈りしながら深さ優先探索(DFS)を行う。
%   その反復でゴールに到達できなければ、枝刈りされた中で最小のf値を次の
%   上限値として再度DFSをやり直す。h(n)が許容的（admissible）であれば、
%   ゴールに到達した時点でのコストは常に最適解と一致する。
%
%   closedリストを持たないため、現在のDFSパス上に既にあるノードへの
%   再訪問のみを禁止する（パス自体を訪問済み集合として使う）。これにより
%   同一パス内での巡回は防げるが、closedリストを使うA*と異なり、異なる
%   パスから同じノードへ重複して到達する探索は防げない。そのためIDA*は
%   最悪ケースでは指数時間となるが、メモリ使用量はO(経路長)と非常に小さい。
%
%   本実装ではヒューリスティックとして Octile distance を用いる。
%       dx = |r1 - r2|, dy = |c1 - c2|
%       h = max(dx, dy) + (sqrt(2) - 1) * min(dx, dy)
%   これは8方向移動（直進コスト1、斜め移動コストsqrt(2)）に対して
%   許容的なヒューリスティックである。
%
%   移動モデル: 上下左右＋斜め4方向の8方向移動。直進コストは1、
%   斜め移動コストはsqrt(2)。コーナーカット禁止：斜め移動する際、
%   その両側の直交マスのいずれかが障害物の場合は斜め移動できない。
%
%   時間複雑度: 最悪ケースで指数時間（closedリストを持たないため重複探索
%               が発生し得る）。反復ごとにDFSをやり直すコストもあるが、
%               深さが浅い反復のノード数は深い反復に比べて少ないため、
%               総コストは最終反復のノード数と同程度のオーダーになる。
%   空間複雑度: O(d)（d=現在のDFSパスの深さ）
%
%   path = IDA_STAR(grid, start, goal)
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

    if isequal(start, goal)
        path = start;
        return;
    end

    sqrt2 = sqrt(2);
    bound = octileDistance(start, goal, sqrt2);

    % pathRows/pathCols: 現在のDFSパスをスタックとして保持する。
    % onPath: パス上に既にあるノードを判定するための論理配列
    %         （closedリストの代わりに、同一パス内の巡回防止だけに使う）。
    pathRows = start(1);
    pathCols = start(2);
    onPath = false(numRows, numCols);
    onPath(start(1), start(2)) = true;

    found = false;
    while ~found
        [pathRows, pathCols, onPath, result] = search( ...
            grid, pathRows, pathCols, onPath, 0, bound, goal, numRows, numCols, sqrt2);

        if result == -1
            found = true;
        elseif ~isfinite(result)
            % どの分岐も上限を超えなかった = ゴールは到達不可能
            path = [];
            return;
        else
            bound = result;
        end
    end

    path = [pathRows', pathCols'];
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

function [pathRows, pathCols, onPath, result] = search( ...
    grid, pathRows, pathCols, onPath, g, bound, goal, numRows, numCols, sqrt2)
% SEARCH 現在のDFSパス（pathRows, pathCols）の末尾ノードから探索を1段進める。
%
%   戻り値resultについて:
%     -1          ゴールに到達した（path*はゴールまでの経路を保持したまま）。
%     -1以外      このブランチで枝刈りされた場合に観測された最小のf値
%                 （次の反復のboundの候補）。何も枝刈りされなければ
%                 +Infを返す。
    dirRow = [-1 1 0 0 -1 -1 1 1];
    dirCol = [0 0 -1 1 -1 1 -1 1];

    row = pathRows(end);
    col = pathCols(end);

    f = g + octileDistance([row col], goal, sqrt2);
    if f > bound
        result = f;
        return;
    end

    if row == goal(1) && col == goal(2)
        result = -1;
        return;
    end

    minExceeded = Inf;

    for d = 1:8
        dr = dirRow(d);
        dc = dirCol(d);
        if ~canMove(grid, row, col, dr, dc, numRows, numCols)
            continue;
        end

        nr = row + dr;
        nc = col + dc;
        if onPath(nr, nc)
            continue; % 現在のDFSパス上に既にあるノードは再訪問しない（巡回防止）
        end

        if dr ~= 0 && dc ~= 0
            stepCost = sqrt2;
        else
            stepCost = 1;
        end

        pathRows(end + 1) = nr; %#ok<AGROW>
        pathCols(end + 1) = nc; %#ok<AGROW>
        onPath(nr, nc) = true;

        [pathRows, pathCols, onPath, childResult] = search( ...
            grid, pathRows, pathCols, onPath, g + stepCost, bound, goal, numRows, numCols, sqrt2);

        if childResult == -1
            result = -1;
            return; % 見つかった経路はpath*に残したまま即座に伝播する
        end

        pathRows(end) = [];
        pathCols(end) = [];
        onPath(nr, nc) = false;

        if childResult < minExceeded
            minExceeded = childResult;
        end
    end

    result = minExceeded;
end
