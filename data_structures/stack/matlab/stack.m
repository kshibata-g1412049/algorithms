function varargout = stack(varargin)
%STACK  スタック（LIFO）データ構造
%
% 使用方法:
%   s = STACK('new')               新しいスタックを生成
%   s = STACK(s, 'push', value)    値をプッシュ
%   [s, v] = STACK(s, 'pop')       値をポップ（変更後のスタックと取り出した値を返す）
%   v = STACK(s, 'peek')           先頭の値を参照（取り出さない）
%   b = STACK(s, 'is_empty')       空かどうかを返す
%   n = STACK(s, 'size')           要素数を返す
%
% 計算量: push/pop/peek/is_empty/size すべて O(1) 均し
% 空状態での pop/peek は error を送出する。

    if nargin == 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        varargout{1} = struct('data', zeros(1, 0), 'n', 0);
        return;
    end

    s = varargin{1};
    op = varargin{2};

    switch op
        case 'push'
            s.data(end+1) = varargin{3}; %#ok<AGROW>
            s.n = s.n + 1;
            varargout{1} = s;

        case 'pop'
            if s.n == 0
                error('stack:underflow', 'stack is empty');
            end
            varargout{2} = s.data(s.n);
            s.data = s.data(1:end-1);
            s.n = s.n - 1;
            varargout{1} = s;

        case 'peek'
            if s.n == 0
                error('stack:underflow', 'stack is empty');
            end
            varargout{1} = s.data(s.n);

        case 'is_empty'
            varargout{1} = s.n == 0;

        case 'size'
            varargout{1} = s.n;

        otherwise
            error('stack:unknownOp', 'unknown operation: %s', op);
    end
end
