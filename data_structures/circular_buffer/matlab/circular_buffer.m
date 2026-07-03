function varargout = circular_buffer(varargin)
%CIRCULAR_BUFFER  循環バッファ（リングバッファ）— 固定容量
%
% 使用方法:
%   b = CIRCULAR_BUFFER('new', capacity)  新しい循環バッファを生成
%   b = CIRCULAR_BUFFER(b, 'push', v)     末尾に値を追加（満杯なら error）
%   [b, v] = CIRCULAR_BUFFER(b, 'pop')    先頭の値を取り出す（空なら error）
%   v = CIRCULAR_BUFFER(b, 'peek')        先頭の値を参照
%   b = CIRCULAR_BUFFER(b, 'is_full')     満杯かどうかを返す
%   b = CIRCULAR_BUFFER(b, 'is_empty')    空かどうかを返す
%   n = CIRCULAR_BUFFER(b, 'size')        要素数を返す
%   n = CIRCULAR_BUFFER(b, 'capacity')    容量を返す

    if nargin >= 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        cap = varargin{2};
        if cap <= 0, error('circular_buffer:invalidArg', 'capacity must be positive'); end
        varargout{1} = struct('data', zeros(1, cap), 'head', 1, 'count', 0, 'cap', cap);
        return;
    end

    b = varargin{1};
    op = varargin{2};

    switch op
        case 'push'
            if b.count == b.cap
                error('circular_buffer:overflow', 'circular buffer is full');
            end
            tail = mod(b.head + b.count - 1, b.cap) + 1;
            b.data(tail) = varargin{3};
            b.count = b.count + 1;
            varargout{1} = b;

        case 'pop'
            if b.count == 0
                error('circular_buffer:underflow', 'circular buffer is empty');
            end
            varargout{2} = b.data(b.head);
            b.head = mod(b.head, b.cap) + 1;
            b.count = b.count - 1;
            varargout{1} = b;

        case 'peek'
            if b.count == 0
                error('circular_buffer:underflow', 'circular buffer is empty');
            end
            varargout{1} = b.data(b.head);

        case 'is_full'
            varargout{1} = b.count == b.cap;

        case 'is_empty'
            varargout{1} = b.count == 0;

        case 'size'
            varargout{1} = b.count;

        case 'capacity'
            varargout{1} = b.cap;

        otherwise
            error('circular_buffer:unknownOp', 'unknown operation: %s', op);
    end
end
