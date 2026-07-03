function varargout = hash_map(varargin)
%HASH_MAP  ハッシュマップ（int→int、チェイン法）
%
%   m = HASH_MAP('new')                   新しいハッシュマップを生成
%   m = HASH_MAP(m, 'insert', key, val)  キーと値を挿入（上書き）
%   v = HASH_MAP(m, 'get', key)          値を取得（なければエラー）
%   [m, b] = HASH_MAP(m, 'remove', key) キーを削除
%   b = HASH_MAP(m, 'contains', key)    キーの存在確認
%   n = HASH_MAP(m, 'size')             要素数
%   b = HASH_MAP(m, 'is_empty')         空かどうか
%
%   実装: cell配列(keys/vals 各16列)＋チェイン法

    CAP = 16;

    if nargin == 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        m = struct('keys', {cell(1, CAP)}, ...
                   'vals', {cell(1, CAP)}, ...
                   'cap',  CAP, ...
                   'sz',   0);
        varargout{1} = m;
        return;
    end

    m  = varargin{1};
    op = varargin{2};

    switch op
        case 'insert'
            key = varargin{3};
            val = varargin{4};
            idx = hm_idx(key, m.cap);
            chain_k = m.keys{idx};
            chain_v = m.vals{idx};
            for i = 1:length(chain_k)
                if chain_k(i) == key
                    chain_v(i) = val;
                    m.vals{idx} = chain_v;
                    varargout{1} = m;
                    return;
                end
            end
            m.keys{idx} = [chain_k, key];
            m.vals{idx} = [chain_v, val];
            m.sz = m.sz + 1;
            varargout{1} = m;

        case 'get'
            key = varargin{3};
            idx = hm_idx(key, m.cap);
            chain_k = m.keys{idx};
            chain_v = m.vals{idx};
            for i = 1:length(chain_k)
                if chain_k(i) == key
                    varargout{1} = chain_v(i);
                    return;
                end
            end
            error('hash_map:keyNotFound', 'key not found: %d', key);

        case 'remove'
            key = varargin{3};
            idx = hm_idx(key, m.cap);
            chain_k = m.keys{idx};
            chain_v = m.vals{idx};
            removed = false;
            for i = 1:length(chain_k)
                if chain_k(i) == key
                    m.keys{idx} = [chain_k(1:i-1), chain_k(i+1:end)];
                    m.vals{idx} = [chain_v(1:i-1), chain_v(i+1:end)];
                    m.sz = m.sz - 1;
                    removed = true;
                    break;
                end
            end
            varargout{1} = m;
            varargout{2} = removed;

        case 'contains'
            key = varargin{3};
            idx = hm_idx(key, m.cap);
            chain_k = m.keys{idx};
            varargout{1} = any(chain_k == key);

        case 'size'
            varargout{1} = m.sz;

        case 'is_empty'
            varargout{1} = (m.sz == 0);

        otherwise
            error('hash_map:unknownOp', 'Unknown operation: %s', op);
    end
end

function idx = hm_idx(key, cap)
    idx = mod(key * 2654435761, cap) + 1;
    if idx < 1, idx = idx + cap; end
end
