function varargout = deque(varargin)
%DEQUE  両端キューデータ構造
%
% 使用方法:
%   d = DEQUE('new')                    新しい両端キューを生成
%   d = DEQUE(d, 'push_front', v)       先頭に値を追加
%   d = DEQUE(d, 'push_back',  v)       末尾に値を追加
%   [d, v] = DEQUE(d, 'pop_front')      先頭の値を取り出す
%   [d, v] = DEQUE(d, 'pop_back')       末尾の値を取り出す
%   v = DEQUE(d, 'peek_front')          先頭の値を参照
%   v = DEQUE(d, 'peek_back')           末尾の値を参照
%   b = DEQUE(d, 'is_empty')            空かどうかを返す
%   n = DEQUE(d, 'size')                要素数を返す

    if nargin == 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        varargout{1} = struct('data', zeros(1, 0), 'n', 0);
        return;
    end

    d = varargin{1};
    op = varargin{2};

    switch op
        case 'push_front'
            d.data = [varargin{3}, d.data];
            d.n = d.n + 1;
            varargout{1} = d;

        case 'push_back'
            d.data(end+1) = varargin{3}; %#ok<AGROW>
            d.n = d.n + 1;
            varargout{1} = d;

        case 'pop_front'
            if d.n == 0, error('deque:underflow', 'deque is empty'); end
            varargout{2} = d.data(1);
            d.data = d.data(2:end);
            d.n = d.n - 1;
            varargout{1} = d;

        case 'pop_back'
            if d.n == 0, error('deque:underflow', 'deque is empty'); end
            varargout{2} = d.data(end);
            d.data = d.data(1:end-1);
            d.n = d.n - 1;
            varargout{1} = d;

        case 'peek_front'
            if d.n == 0, error('deque:underflow', 'deque is empty'); end
            varargout{1} = d.data(1);

        case 'peek_back'
            if d.n == 0, error('deque:underflow', 'deque is empty'); end
            varargout{1} = d.data(end);

        case 'is_empty'
            varargout{1} = d.n == 0;

        case 'size'
            varargout{1} = d.n;

        otherwise
            error('deque:unknownOp', 'unknown operation: %s', op);
    end
end
