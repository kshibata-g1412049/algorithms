function varargout = trie(varargin)
%TRIE  トライ木（前置木、並列配列＋整数インデックス）
%
%   t = TRIE('new')                   新しいトライを生成
%   t = TRIE(t, 'insert', word)       単語を挿入
%   b = TRIE(t, 'search', word)       単語を検索
%   b = TRIE(t, 'starts_with', pfx)   プレフィックス検索
%   [t, b] = TRIE(t, 'remove', word)  単語を削除
%   n = TRIE(t, 'size')               単語数
%
%   ノードはchildren(26,MAX)で表現。小文字英字a-zのみ対応。

    MAX  = 4096;
    ALPHA = 26;

    if nargin == 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        t = struct('children', zeros(ALPHA, MAX), ...
                   'is_end',   false(1, MAX), ...
                   'cnt',      zeros(1, MAX), ...
                   'free',     2, ...
                   'sz',       0);
        % node 1 = root
        varargout{1} = t;
        return;
    end

    t  = varargin{1};
    op = varargin{2};

    switch op
        case 'insert'
            word = varargin{3};
            [t, ~] = trie_insert(t, 1, word, 1);
            varargout{1} = t;

        case 'search'
            word = varargin{3};
            varargout{1} = trie_search(t, 1, word, 1);

        case 'starts_with'
            pfx = varargin{3};
            varargout{1} = trie_prefix(t, 1, pfx, 1);

        case 'remove'
            word = varargin{3};
            [t, ~, removed] = trie_remove(t, 1, word, 1);
            if removed, t.sz = t.sz - 1; end
            varargout{1} = t;
            varargout{2} = removed;

        case 'size'
            varargout{1} = t.sz;

        otherwise
            error('trie:unknownOp', 'Unknown operation: %s', op);
    end
end

function [t, node] = trie_insert(t, node, word, pos)
    if pos > length(word)
        if ~t.is_end(node)
            t.is_end(node) = true;
            t.sz = t.sz + 1;
        end
        return;
    end
    idx = double(word(pos)) - double('a') + 1;
    child = t.children(idx, node);
    if child == 0
        child = t.free; t.free = t.free + 1;
        t.children(idx, node) = child;
    end
    t.cnt(child) = t.cnt(child) + 1;
    [t, ~] = trie_insert(t, child, word, pos + 1);
    node_out = node; %#ok<NASGU>
end

function found = trie_search(t, node, word, pos)
    if pos > length(word)
        found = t.is_end(node);
        return;
    end
    idx = double(word(pos)) - double('a') + 1;
    child = t.children(idx, node);
    if child == 0, found = false; return; end
    found = trie_search(t, child, word, pos + 1);
end

function found = trie_prefix(t, node, pfx, pos)
    if pos > length(pfx), found = true; return; end
    idx = double(pfx(pos)) - double('a') + 1;
    child = t.children(idx, node);
    if child == 0, found = false; return; end
    found = trie_prefix(t, child, pfx, pos + 1);
end

function [t, should_del, removed] = trie_remove(t, node, word, pos)
    removed = false;
    should_del = false;
    if pos > length(word)
        if ~t.is_end(node), return; end
        t.is_end(node) = false;
        removed = true;
        should_del = (t.cnt(node) == 0);
        return;
    end
    idx = double(word(pos)) - double('a') + 1;
    child = t.children(idx, node);
    if child == 0, return; end
    [t, child_del, removed] = trie_remove(t, child, word, pos + 1);
    if removed
        t.cnt(child) = t.cnt(child) - 1;
    end
    if child_del
        t.children(idx, node) = 0;
    end
    should_del = ~t.is_end(node) && (t.cnt(node) == 0) && removed;
end
