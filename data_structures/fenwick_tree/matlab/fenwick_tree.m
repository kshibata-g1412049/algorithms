function varargout = fenwick_tree(varargin)
%FENWICK_TREE  フェニック木 (Binary Indexed Tree)
%
%   ft = FENWICK_TREE('new', n)               サイズ n の空BITを生成
%   ft = FENWICK_TREE('from_data', data)      配列から構築
%   ft = FENWICK_TREE(ft, 'update', i, delta) i番目（1始まり）に delta を加算
%   s  = FENWICK_TREE(ft, 'prefix_sum', i)    [1, i] の前置和
%   s  = FENWICK_TREE(ft, 'range_sum', l, r)  [l, r] の区間和
%   n  = FENWICK_TREE(ft, 'size')             要素数
%
%   注: LSBは i - bitand(int32(i), int32(i-1)) で計算（Octave互換）。

    if ischar(varargin{1})
        cmd = varargin{1};
        if strcmp(cmd, 'new')
            n = varargin{2};
            ft = struct('tree', zeros(1, n+1), 'n', n);
            varargout{1} = ft;
            return;
        elseif strcmp(cmd, 'from_data')
            data = varargin{2};
            n = length(data);
            ft = struct('tree', zeros(1, n+1), 'n', n);
            for i = 1:n
                ft = bit_update(ft, i, data(i));
            end
            varargout{1} = ft;
            return;
        end
    end

    ft = varargin{1};
    op = varargin{2};

    switch op
        case 'update'
            i     = varargin{3};
            delta = varargin{4};
            if i < 1 || i > ft.n
                error('fenwick_tree:outOfRange', 'index out of range');
            end
            ft = bit_update(ft, i, delta);
            varargout{1} = ft;

        case 'prefix_sum'
            i = varargin{3};
            if i < 0 || i > ft.n
                error('fenwick_tree:outOfRange', 'index out of range');
            end
            varargout{1} = bit_prefix(ft, i);

        case 'range_sum'
            l = varargin{3};
            r = varargin{4};
            if l < 1 || r > ft.n || l > r
                error('fenwick_tree:outOfRange', 'invalid range');
            end
            varargout{1} = bit_prefix(ft, r) - bit_prefix(ft, l-1);

        case 'size'
            varargout{1} = ft.n;

        otherwise
            error('fenwick_tree:unknownOp', 'Unknown operation: %s', op);
    end
end

function ft = bit_update(ft, i, delta)
    while i <= ft.n
        ft.tree(i) = ft.tree(i) + delta;
        i = i + lsb(i);
    end
end

function s = bit_prefix(ft, i)
    s = 0;
    while i > 0
        s = s + ft.tree(i);
        i = i - lsb(i);
    end
end

function v = lsb(i)
    % Lowest set bit: i & (-i) = i - (i & (i-1)), Octave-safe
    v = double(i - bitand(int32(i), int32(i - 1)));
end
