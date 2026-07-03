function varargout = queue(varargin)
%QUEUE  キュー（FIFO）データ構造
%
% 使用方法:
%   q = QUEUE('new')               新しいキューを生成
%   q = QUEUE(q, 'enqueue', v)     末尾に値を追加
%   [q, v] = QUEUE(q, 'dequeue')   先頭の値を取り出す
%   v = QUEUE(q, 'peek')           先頭の値を参照（取り出さない）
%   b = QUEUE(q, 'is_empty')       空かどうかを返す
%   n = QUEUE(q, 'size')           要素数を返す
%
% 計算量: すべての操作 O(1)
% 空状態での dequeue/peek は error を送出する。

    if nargin == 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        varargout{1} = struct('data', zeros(1, 0), 'n', 0);
        return;
    end

    q = varargin{1};
    op = varargin{2};

    switch op
        case 'enqueue'
            q.data(end+1) = varargin{3}; %#ok<AGROW>
            q.n = q.n + 1;
            varargout{1} = q;

        case 'dequeue'
            if q.n == 0
                error('queue:underflow', 'queue is empty');
            end
            varargout{2} = q.data(1);
            q.data = q.data(2:end);
            q.n = q.n - 1;
            varargout{1} = q;

        case 'peek'
            if q.n == 0
                error('queue:underflow', 'queue is empty');
            end
            varargout{1} = q.data(1);

        case 'is_empty'
            varargout{1} = q.n == 0;

        case 'size'
            varargout{1} = q.n;

        otherwise
            error('queue:unknownOp', 'unknown operation: %s', op);
    end
end
