function varargout = binary_heap(varargin)
%BINARY_HEAP  二分ヒープ（最小ヒープ、配列バック）
%
%   h = BINARY_HEAP('new')              新しいヒープを生成
%   h = BINARY_HEAP(h, 'insert', v)    値を挿入
%   v = BINARY_HEAP(h, 'find_min')     最小値を参照
%   [h, v] = BINARY_HEAP(h, 'extract_min') 最小値を取り出す
%   n = BINARY_HEAP(h, 'size')         要素数
%   b = BINARY_HEAP(h, 'is_empty')     空かどうか

    if nargin == 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        varargout{1} = struct('data', zeros(1, 0), 'n', 0);
        return;
    end

    h  = varargin{1};
    op = varargin{2};

    switch op
        case 'insert'
            v = varargin{3};
            h.n = h.n + 1;
            h.data(h.n) = v;
            h = heap_sift_up(h, h.n);
            varargout{1} = h;

        case 'find_min'
            if h.n == 0
                error('binary_heap:underflow', 'heap is empty');
            end
            varargout{1} = h.data(1);

        case 'extract_min'
            if h.n == 0
                error('binary_heap:underflow', 'heap is empty');
            end
            v = h.data(1);
            h.data(1) = h.data(h.n);
            h.n = h.n - 1;
            if h.n > 0
                h = heap_sift_down(h, 1);
            end
            varargout{1} = h;
            varargout{2} = v;

        case 'size'
            varargout{1} = h.n;

        case 'is_empty'
            varargout{1} = (h.n == 0);

        otherwise
            error('binary_heap:unknownOp', 'Unknown operation: %s', op);
    end
end

function h = heap_sift_up(h, i)
    while i > 1
        p = floor(i / 2);
        if h.data(p) <= h.data(i), break; end
        tmp = h.data(p); h.data(p) = h.data(i); h.data(i) = tmp;
        i = p;
    end
end

function h = heap_sift_down(h, i)
    while true
        smallest = i;
        l = 2 * i;
        r = 2 * i + 1;
        if l <= h.n && h.data(l) < h.data(smallest), smallest = l; end
        if r <= h.n && h.data(r) < h.data(smallest), smallest = r; end
        if smallest == i, break; end
        tmp = h.data(i); h.data(i) = h.data(smallest); h.data(smallest) = tmp;
        i = smallest;
    end
end
