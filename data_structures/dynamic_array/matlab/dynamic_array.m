function varargout = dynamic_array(varargin)
%DYNAMIC_ARRAY  動的配列（倍増戦略）
%
% 使用方法:
%   a = DYNAMIC_ARRAY('new')           新しい動的配列を生成
%   a = DYNAMIC_ARRAY(a, 'append', v)  末尾に値を追加  O(1) 均し
%   v = DYNAMIC_ARRAY(a, 'get', i)     i番目の要素を返す（1始まり）  O(1)
%   a = DYNAMIC_ARRAY(a, 'set', i, v)  i番目の要素を設定（1始まり）  O(1)
%   a = DYNAMIC_ARRAY(a, 'insert', i, v) i番目の位置に挿入（1始まり）  O(n)
%   a = DYNAMIC_ARRAY(a, 'remove', i)  i番目の要素を削除（1始まり）  O(n)
%   n = DYNAMIC_ARRAY(a, 'size')       要素数を返す
%   n = DYNAMIC_ARRAY(a, 'capacity')   確保済み容量を返す

    if nargin == 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        varargout{1} = struct('data', zeros(1, 4), 'n', 0, 'cap', 4);
        return;
    end

    a = varargin{1};
    op = varargin{2};

    switch op
        case 'append'
            if a.n == a.cap
                a.data = [a.data, zeros(1, a.cap)];
                a.cap = a.cap * 2;
            end
            a.n = a.n + 1;
            a.data(a.n) = varargin{3};
            varargout{1} = a;

        case 'get'
            idx = varargin{3};
            if idx < 1 || idx > a.n
                error('dynamic_array:outOfRange', 'index out of range');
            end
            varargout{1} = a.data(idx);

        case 'set'
            idx = varargin{3};
            if idx < 1 || idx > a.n
                error('dynamic_array:outOfRange', 'index out of range');
            end
            a.data(idx) = varargin{4};
            varargout{1} = a;

        case 'insert'
            idx = varargin{3};
            if idx < 1 || idx > a.n + 1
                error('dynamic_array:outOfRange', 'index out of range');
            end
            if a.n == a.cap
                a.data = [a.data, zeros(1, a.cap)];
                a.cap = a.cap * 2;
            end
            a.data(idx+1:a.n+1) = a.data(idx:a.n);
            a.data(idx) = varargin{4};
            a.n = a.n + 1;
            varargout{1} = a;

        case 'remove'
            idx = varargin{3};
            if idx < 1 || idx > a.n
                error('dynamic_array:outOfRange', 'index out of range');
            end
            a.data(idx:a.n-1) = a.data(idx+1:a.n);
            a.n = a.n - 1;
            varargout{1} = a;

        case 'size'
            varargout{1} = a.n;

        case 'capacity'
            varargout{1} = a.cap;

        otherwise
            error('dynamic_array:unknownOp', 'unknown operation: %s', op);
    end
end
