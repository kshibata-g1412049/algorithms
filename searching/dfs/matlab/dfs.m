function order = dfs(adjacency, start)
% DFS 深さ優先探索（Depth-First Search）でグラフを探索する
%
%   グラフを開始ノードから「行き止まりになるまで深く」探索するアルゴリズム。
%   あるノードを訪問したら、まだ訪問していない最初の隣接ノード（隣接リストの
%   並び順で最初に見つかった未訪問ノード）へ進み、それを再帰的に繰り返す。
%   これ以上進めなくなったら直前のノードに戻り（バックトラック）、
%   他に未訪問の隣接ノードがあればそちらへ進む。
%
%   訪問済みノードを管理する配列を使うことで、サイクルがあっても
%   同じノードを2回訪問せず、無限ループにならない。
%
%   グラフはセル配列の隣接リスト（adjacency{i}がノードiの隣接ノード番号の
%   ベクトル）で表現する。MATLABは1始まりであるため、ノード番号は
%   すべて1始まりとして扱う（例: adjacency = {[2,3], [1,4], [1,4], [2,3]}）。
%
%   時間複雑度: O(V + E)（Vはノード数、Eは辺数）
%   空間複雑度: O(V)（訪問済み配列と再帰呼び出しスタックのため）
%
%   order = DFS(adjacency, start) は、startから到達可能なノードの
%   訪問順序（行ベクトル）を返す。startから到達できないノードは
%   結果に含まれない。

    n = length(adjacency);
    order = [];

    if start < 1 || start > n
        % 範囲外の開始ノードは訪問順序なしとして空を返す
        return;
    end

    visited = false(1, n);
    [order, ~] = visit(adjacency, start, visited, order);
end

function [order, visited] = visit(adjacency, current, visited, order)
% currentから再帰的に未訪問の隣接ノードをたどるヘルパー関数。
% 隣接リストの並び順で最初に見つかった未訪問ノードへ進む。
% MATLABの配列は値渡しのため、visitedの更新を呼び出し元へ反映するには
% 戻り値として返す必要がある。
    visited(current) = true;
    order(end + 1) = current;

    neighbors = adjacency{current};
    for k = 1:length(neighbors)
        neighbor = neighbors(k);
        if ~visited(neighbor)
            [order, visited] = visit(adjacency, neighbor, visited, order);
        end
    end
end
