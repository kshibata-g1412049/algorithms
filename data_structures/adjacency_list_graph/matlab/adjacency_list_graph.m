function varargout = adjacency_list_graph(varargin)
%ADJACENCY_LIST_GRAPH  隣接リストグラフ（有向グラフ）
%
%   g = ADJACENCY_LIST_GRAPH('new', max_v)    最大頂点数を指定して生成
%   g = ADJACENCY_LIST_GRAPH(g, 'add_vertex', v)   頂点を追加
%   g = ADJACENCY_LIST_GRAPH(g, 'add_edge', u, v)  辺を追加
%   b = ADJACENCY_LIST_GRAPH(g, 'has_edge', u, v)  辺の存在確認
%   a = ADJACENCY_LIST_GRAPH(g, 'get_neighbors', v) 隣接頂点リスト
%   b = ADJACENCY_LIST_GRAPH(g, 'has_vertex', v)   頂点の存在確認
%   n = ADJACENCY_LIST_GRAPH(g, 'vertex_count')    頂点数
%   n = ADJACENCY_LIST_GRAPH(g, 'edge_count')      辺数
%
%   実装: cell配列(adj)で各頂点の隣接リストを保持。
%         頂点IDはインデックス(1始まり)に変換。

    if nargin >= 2 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        max_v = varargin{2};
        g = struct('adj',       {cell(1, max_v)}, ...
                   'has_v',     false(1, max_v), ...
                   'max_v',     max_v, ...
                   'n_vert',    0, ...
                   'n_edge',    0);
        varargout{1} = g;
        return;
    end

    g  = varargin{1};
    op = varargin{2};

    switch op
        case 'add_vertex'
            v = varargin{3} + 1;
            if ~g.has_v(v)
                g.has_v(v) = true;
                g.adj{v}   = zeros(1, 0);
                g.n_vert   = g.n_vert + 1;
            end
            varargout{1} = g;

        case 'add_edge'
            u = varargin{3} + 1;
            v = varargin{4} + 1;
            if ~g.has_v(u)
                g.has_v(u) = true; g.adj{u} = zeros(1,0); g.n_vert = g.n_vert+1;
            end
            if ~g.has_v(v)
                g.has_v(v) = true; g.adj{v} = zeros(1,0); g.n_vert = g.n_vert+1;
            end
            g.adj{u}(end+1) = v - 1; %#ok<AGROW>
            g.n_edge = g.n_edge + 1;
            varargout{1} = g;

        case 'has_edge'
            u = varargin{3} + 1;
            v = varargin{4};
            if ~g.has_v(u), varargout{1} = false; return; end
            varargout{1} = any(g.adj{u} == v);

        case 'get_neighbors'
            v = varargin{3} + 1;
            if ~g.has_v(v)
                error('adjacency_list_graph:noVertex', 'vertex not found: %d', v-1);
            end
            varargout{1} = g.adj{v};

        case 'has_vertex'
            v = varargin{3} + 1;
            varargout{1} = (v >= 1 && v <= g.max_v && g.has_v(v));

        case 'vertex_count'
            varargout{1} = g.n_vert;

        case 'edge_count'
            varargout{1} = g.n_edge;

        otherwise
            error('adjacency_list_graph:unknownOp', 'Unknown operation: %s', op);
    end
end
