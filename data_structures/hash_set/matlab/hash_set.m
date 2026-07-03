function varargout = hash_set(varargin)
%HASH_SET  ハッシュセット（線形探索法）
%
%   s = HASH_SET('new')               新しいハッシュセットを生成
%   s = HASH_SET(s, 'insert', v)     値を挿入
%   b = HASH_SET(s, 'contains', v)   値の存在確認
%   [s, b] = HASH_SET(s, 'remove', v) 値を削除
%   n = HASH_SET(s, 'size')          要素数
%   b = HASH_SET(s, 'is_empty')      空かどうか
%
%   state: 0=EMPTY, 1=USED, 2=DELETED

    CAP = 32;

    if nargin == 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        s = struct('table', zeros(1, CAP), ...
                   'state', zeros(1, CAP), ...
                   'cap',   CAP, ...
                   'sz',    0);
        varargout{1} = s;
        return;
    end

    s  = varargin{1};
    op = varargin{2};

    switch op
        case 'insert'
            v = varargin{3};
            i = hs_probe(s, v);
            if s.state(i) ~= 1
                s.table(i) = v;
                s.state(i) = 1;
                s.sz = s.sz + 1;
            end
            varargout{1} = s;

        case 'contains'
            v = varargin{3};
            i = hs_probe(s, v);
            varargout{1} = (s.state(i) == 1 && s.table(i) == v);

        case 'remove'
            v = varargin{3};
            i = hs_probe(s, v);
            if s.state(i) == 1 && s.table(i) == v
                s.state(i) = 2;
                s.sz = s.sz - 1;
                removed = true;
            else
                removed = false;
            end
            varargout{1} = s;
            varargout{2} = removed;

        case 'size'
            varargout{1} = s.sz;

        case 'is_empty'
            varargout{1} = (s.sz == 0);

        otherwise
            error('hash_set:unknownOp', 'Unknown operation: %s', op);
    end
end

function i = hs_probe(s, v)
    h = mod(v * 2654435761, s.cap);
    i = h + 1;
    if i < 1, i = i + s.cap; end
    while s.state(i) == 1 && s.table(i) ~= v
        i = mod(i, s.cap) + 1;
    end
end
