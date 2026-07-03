function varargout = segment_tree(varargin)
%SEGMENT_TREE  セグメント木（区間和）
%
%   st = SEGMENT_TREE('new', data)          配列からセグメント木を構築
%   st = SEGMENT_TREE(st, 'update', i, v)  i番目（1始まり）を更新
%   s  = SEGMENT_TREE(st, 'query', l, r)   [l, r]（1始まり）の区間和
%   n  = SEGMENT_TREE(st, 'size')          要素数

    if nargin >= 2 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        data = varargin{2};
        n = length(data);
        tree = zeros(1, 4 * n);
        st = struct('tree', tree, 'n', n);
        if n > 0
            st = seg_build(st, data, 1, 1, n);
        end
        varargout{1} = st;
        return;
    end

    st = varargin{1};
    op = varargin{2};

    switch op
        case 'update'
            i = varargin{3};
            v = varargin{4};
            if i < 1 || i > st.n
                error('segment_tree:outOfRange', 'index out of range');
            end
            st = seg_update(st, 1, 1, st.n, i, v);
            varargout{1} = st;

        case 'query'
            l = varargin{3};
            r = varargin{4};
            if l < 1 || r > st.n || l > r
                error('segment_tree:outOfRange', 'invalid range');
            end
            varargout{1} = seg_query(st, 1, 1, st.n, l, r);

        case 'size'
            varargout{1} = st.n;

        otherwise
            error('segment_tree:unknownOp', 'Unknown operation: %s', op);
    end
end

function st = seg_build(st, data, node, lo, hi)
    if lo == hi
        st.tree(node) = data(lo);
        return;
    end
    mid = floor((lo + hi) / 2);
    st = seg_build(st, data, 2*node, lo, mid);
    st = seg_build(st, data, 2*node+1, mid+1, hi);
    st.tree(node) = st.tree(2*node) + st.tree(2*node+1);
end

function st = seg_update(st, node, lo, hi, i, v)
    if lo == hi
        st.tree(node) = v;
        return;
    end
    mid = floor((lo + hi) / 2);
    if i <= mid
        st = seg_update(st, 2*node, lo, mid, i, v);
    else
        st = seg_update(st, 2*node+1, mid+1, hi, i, v);
    end
    st.tree(node) = st.tree(2*node) + st.tree(2*node+1);
end

function s = seg_query(st, node, lo, hi, l, r)
    if r < lo || hi < l, s = 0; return; end
    if l <= lo && hi <= r, s = st.tree(node); return; end
    mid = floor((lo + hi) / 2);
    s = seg_query(st, 2*node, lo, mid, l, r) + seg_query(st, 2*node+1, mid+1, hi, l, r);
end
