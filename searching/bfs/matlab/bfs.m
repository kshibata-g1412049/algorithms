function order = bfs(adjacency, start)
% BFS 幅優先探索（Breadth-First Search）でグラフを探索する
%
%   グラフを開始ノードから「近いノードから順に」探索するアルゴリズム。
%   キュー（FIFO）を使い、開始ノードをキューに入れた後、キューの先頭から
%   ノードを取り出して隣接ノードを調べ、未訪問の隣接ノードを隣接リストの
%   並び順でキューに追加していく。これを繰り返すことで、開始ノードからの
%   距離が近い順にノードを訪問する。
%
%   訪問済みノードを管理する配列を使うことで、サイクルがあっても
%   同じノードを2回訪問せず、無限ループにならない。
%
%   グラフはセル配列の隣接リスト（adjacency{i}がノードiの隣接ノード番号の
%   ベクトル）で表現する。MATLABは1始まりであるため、ノード番号は
%   すべて1始まりとして扱う（例: adjacency = {[2,3], [1,4], [1,4], [2,3]}）。
%
%   時間複雑度: O(V + E)（Vはノード数、Eは辺数）
%   空間複雑度: O(V)（訪問済み配列とキューのため）
%
%   order = BFS(adjacency, start) は、startから到達可能なノードの
%   訪問順序（行ベクトル）を返す。startから到達できないノードは
%   結果に含まれない。

    n = length(adjacency);
    order = [];

    if start < 1 || start > n
        % 範囲外の開始ノードは訪問順序なしとして空を返す
        return;
    end

    visited = false(1, n);
    queue = [start];
    visited(start) = true;

    while ~isempty(queue)
        current = queue(1);
        queue(1) = [];
        order(end + 1) = current; %#ok<AGROW>

        % 隣接リストの並び順で未訪問の隣接ノードをキューに追加する
        neighbors = adjacency{current};
        for k = 1:length(neighbors)
            neighbor = neighbors(k);
            if ~visited(neighbor)
                visited(neighbor) = true;
                queue(end + 1) = neighbor; %#ok<AGROW>
            end
        end
    end
end
