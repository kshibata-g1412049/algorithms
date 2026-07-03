function varargout = bloom_filter(varargin)
%BLOOM_FILTER  ブルームフィルタ
%
%   bf = BLOOM_FILTER('new', m, k)     m ビット、k ハッシュ関数のフィルタ生成
%   bf = BLOOM_FILTER(bf, 'insert', v) 値を挿入
%   b  = BLOOM_FILTER(bf, 'contains', v) 値を確認（偽陽性あり・偽陰性なし）

    if nargin >= 3 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        m = varargin{2};
        k = varargin{3};
        bf = struct('bits', false(1, m), 'm', m, 'k', k);
        varargout{1} = bf;
        return;
    end

    bf = varargin{1};
    op = varargin{2};

    switch op
        case 'insert'
            v = varargin{3};
            for i = 0:bf.k-1
                idx = bf_hash(v, i, bf.m) + 1;
                bf.bits(idx) = true;
            end
            varargout{1} = bf;

        case 'contains'
            v = varargin{3};
            found = true;
            for i = 0:bf.k-1
                idx = bf_hash(v, i, bf.m) + 1;
                if ~bf.bits(idx), found = false; break; end
            end
            varargout{1} = found;

        otherwise
            error('bloom_filter:unknownOp', 'Unknown operation: %s', op);
    end
end

function h = bf_hash(value, seed, m)
    h = bitxor(int32(value), int32(seed * int32(hex2dec('9E3779B9'))));
    h = uint32(h);
    h = bitxor(h, bitshift(h, -16));
    h = bitand(uint32(h) * uint32(hex2dec('85EBCA6B')), uint32(hex2dec('FFFFFFFF')));
    h = bitxor(h, bitshift(h, -13));
    h = bitand(uint32(h) * uint32(hex2dec('C2B2AE35')), uint32(hex2dec('FFFFFFFF')));
    h = bitxor(h, bitshift(h, -16));
    h = double(mod(h, uint32(m)));
end
