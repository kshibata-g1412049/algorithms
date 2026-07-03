function varargout = adjacency_matrix_graph(varargin)
%ADJACENCY_MATRIX_GRAPH  隣接行列グラフ（有向グラフ、固定頂点数）
%
%   g = ADJACENCY_MATRIX_GRAPH('new', n)             n頂点グラフを生成
%   g = ADJACENCY_MATRIX_GRAPH(g, 'add_edge', u, v)    辺を追加
%   g = ADJACENCY_MATRIX_GRAPH(g, 'remove_edge', u, v) 辺を削除
%   b = ADJACENCY_MATRIX_GRAPH(g, 'has_edge', u, v)    辺の存在確認
%   a = ADJACENCY_MATRIX_GRAPH(g, 'get_neighbors', v)  隣接頂点リスト
%   n = ADJACENCY_MATRIX_GRAPH(g, 'vertex_count')      頂点数
%   n = ADJACENCY_MATRIX_GRAPH(g, 'edge_count')        辺数
%
%   頂点IDは0始まり（内部で1始まりに変換）。

    if nargin >= 2 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        n = varargin{2};
        g = struct('mat',    false(n, n), ...
                   'n',      n, ...
                   'n_edge', 0);
        varargout{1} = g;
        return;
    end

    g  = varargin{1};
    op = varargin{2};

    switch op
        case 'add_edge'
            u = varargin{3} + 1;
            v = varargin{4} + 1;
            if u < 1 || u > g.n || v < 1 || v > g.n
                error('adjacency_matrix_graph:outOfRange', 'vertex out of range');
            end
            if ~g.mat(u, v)
                g.mat(u, v) = true;
                g.n_edge = g.n_edge + 1;
            end
            varargout{1} = g;

        case 'remove_edge'
            u = varargin{3} + 1;
            v = varargin{4} + 1;
            if g.mat(u, v)
                g.mat(u, v) = false;
                g.n_edge = g.n_edge - 1;
            end
            varargout{1} = g;

        case 'has_edge'
            u = varargin{3} + 1;
            v = varargin{4} + 1;
            if u < 1 || u > g.n || v < 1 || v > g.n
                error('adjacency_matrix_graph:outOfRange', 'vertex out of range');
            end
            varargout{1} = g.mat(u, v);

        case 'get_neighbors'
            v = varargin{3} + 1;
            nb = find(g.mat(v, :)) - 1;
            varargout{1} = nb;

        case 'vertex_count'
            varargout{1} = g.n;

        case 'edge_count'
            varargout{1} = g.n_edge;

        otherwise
            error('adjacency_matrix_graph:unknownOp', 'Unknown operation: %s', op);
    end
end
