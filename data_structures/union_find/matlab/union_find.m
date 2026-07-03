function varargout = union_find(varargin)
%UNION_FIND  素集合データ構造（経路圧縮＋ランク結合）
%
%   uf = UNION_FIND('new', n)              n要素のUFを生成（0始まり）
%   [uf, r] = UNION_FIND(uf, 'find', x)  代表元を返す（経路圧縮）
%   [uf, b] = UNION_FIND(uf, 'unite', x, y) 合併（成功:true）
%   [uf, b] = UNION_FIND(uf, 'connected', x, y) 同じ集合か
%   n = UNION_FIND(uf, 'size')           要素数
%   n = UNION_FIND(uf, 'components')     連結成分数

    if nargin >= 2 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        n = varargin{2};
        uf = struct('parent', 1:n, ...
                    'rnk',    zeros(1, n), ...
                    'n',      n, ...
                    'comp',   n);
        varargout{1} = uf;
        return;
    end

    uf = varargin{1};
    op = varargin{2};

    switch op
        case 'find'
            x = varargin{3} + 1;
            [uf, r] = uf_find(uf, x);
            varargout{1} = uf;
            varargout{2} = r - 1;

        case 'unite'
            x = varargin{3} + 1;
            y = varargin{4} + 1;
            [uf, rx] = uf_find(uf, x);
            [uf, ry] = uf_find(uf, y);
            if rx == ry
                varargout{1} = uf;
                varargout{2} = false;
                return;
            end
            if uf.rnk(rx) < uf.rnk(ry), tmp=rx; rx=ry; ry=tmp; end
            uf.parent(ry) = rx;
            if uf.rnk(rx) == uf.rnk(ry), uf.rnk(rx) = uf.rnk(rx) + 1; end
            uf.comp = uf.comp - 1;
            varargout{1} = uf;
            varargout{2} = true;

        case 'connected'
            x = varargin{3} + 1;
            y = varargin{4} + 1;
            [uf, rx] = uf_find(uf, x);
            [uf, ry] = uf_find(uf, y);
            varargout{1} = uf;
            varargout{2} = (rx == ry);

        case 'size'
            varargout{1} = uf.n;

        case 'components'
            varargout{1} = uf.comp;

        otherwise
            error('union_find:unknownOp', 'Unknown operation: %s', op);
    end
end

function [uf, root] = uf_find(uf, x)
    if uf.parent(x) ~= x
        [uf, uf.parent(x)] = uf_find(uf, uf.parent(x));
    end
    root = uf.parent(x);
end
