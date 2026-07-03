function varargout = binary_search_tree(varargin)
%BINARY_SEARCH_TREE  二分探索木（並列配列＋整数インデックス）
%
%   t = BINARY_SEARCH_TREE('new')            新しいBSTを生成
%   t = BINARY_SEARCH_TREE(t, 'insert', v)   値を挿入（重複無視）
%   b = BINARY_SEARCH_TREE(t, 'search', v)   値を検索
%   [t, b] = BINARY_SEARCH_TREE(t, 'remove', v) 値を削除
%   v = BINARY_SEARCH_TREE(t, 'min')         最小値
%   v = BINARY_SEARCH_TREE(t, 'max')         最大値
%   a = BINARY_SEARCH_TREE(t, 'inorder')     中順走査
%   a = BINARY_SEARCH_TREE(t, 'preorder')    前順走査
%   a = BINARY_SEARCH_TREE(t, 'postorder')   後順走査
%   n = BINARY_SEARCH_TREE(t, 'size')        要素数
%   b = BINARY_SEARCH_TREE(t, 'is_empty')    空かどうか

    MAX = 1024;

    if nargin == 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        t = struct('val',   zeros(1, MAX), ...
                   'left',  zeros(1, MAX), ...
                   'right', zeros(1, MAX), ...
                   'root',  0, ...
                   'free',  1, ...
                   'sz',    0);
        varargout{1} = t;
        return;
    end

    t  = varargin{1};
    op = varargin{2};

    switch op
        case 'insert'
            v = varargin{3};
            [t, ~] = bst_insert(t, t.root, v, MAX);
            varargout{1} = t;

        case 'search'
            v = varargin{3};
            varargout{1} = bst_search(t, t.root, v);

        case 'remove'
            v = varargin{3};
            [t, removed] = bst_remove(t, t.root, v, 0);
            varargout{1} = t;
            varargout{2} = removed;

        case 'min'
            if t.root == 0
                error('binary_search_tree:underflow', 'tree is empty');
            end
            cur = t.root;
            while t.left(cur) ~= 0, cur = t.left(cur); end
            varargout{1} = t.val(cur);

        case 'max'
            if t.root == 0
                error('binary_search_tree:underflow', 'tree is empty');
            end
            cur = t.root;
            while t.right(cur) ~= 0, cur = t.right(cur); end
            varargout{1} = t.val(cur);

        case 'inorder'
            out = zeros(1, t.sz);
            [~, out] = bst_inorder(t, t.root, out, 1);
            varargout{1} = out;

        case 'preorder'
            out = zeros(1, t.sz);
            [~, out] = bst_preorder(t, t.root, out, 1);
            varargout{1} = out;

        case 'postorder'
            out = zeros(1, t.sz);
            [~, out] = bst_postorder(t, t.root, out, 1);
            varargout{1} = out;

        case 'size'
            varargout{1} = t.sz;

        case 'is_empty'
            varargout{1} = (t.sz == 0);

        otherwise
            error('binary_search_tree:unknownOp', 'Unknown operation: %s', op);
    end
end

function [t, node_idx] = bst_insert(t, node, v, MAX)
    if node == 0
        idx = t.free; t.free = t.free + 1;
        t.val(idx)   = v;
        t.left(idx)  = 0;
        t.right(idx) = 0;
        t.sz = t.sz + 1;
        node_idx = idx;
        if t.root == 0, t.root = idx; end
    elseif v < t.val(node)
        [t, child] = bst_insert(t, t.left(node), v, MAX);
        t.left(node) = child;
        node_idx = node;
    elseif v > t.val(node)
        [t, child] = bst_insert(t, t.right(node), v, MAX);
        t.right(node) = child;
        node_idx = node;
    else
        node_idx = node;
    end
end

function found = bst_search(t, node, v)
    found = false;
    while node ~= 0
        if v == t.val(node), found = true; return; end
        if v < t.val(node), node = t.left(node);
        else,               node = t.right(node); end
    end
end

function [t, removed] = bst_remove(t, node, v, parent)
    removed = false;
    if node == 0, return; end
    if v < t.val(node)
        [t, removed] = bst_remove(t, t.left(node), v, node);
        if removed && t.left(node) ~= 0
            % child pointer already updated in recursive call
        end
    elseif v > t.val(node)
        [t, removed] = bst_remove(t, t.right(node), v, node);
    else
        removed = true;
        t.sz = t.sz - 1;
        if t.left(node) == 0 && t.right(node) == 0
            replacement = 0;
        elseif t.left(node) == 0
            replacement = t.right(node);
        elseif t.right(node) == 0
            replacement = t.left(node);
        else
            % find in-order successor
            succ = t.right(node);
            while t.left(succ) ~= 0, succ = t.left(succ); end
            t.val(node) = t.val(succ);
            % remove successor (it has no left child)
            [t, ~] = bst_remove_node(t, t.right(node), t.val(succ), node, false);
            return;
        end
        if t.root == node
            t.root = replacement;
        elseif t.left(parent) == node
            t.left(parent) = replacement;
        else
            t.right(parent) = replacement;
        end
    end
end

function [t, done] = bst_remove_node(t, node, v, parent, is_left)
    done = false;
    if node == 0, return; end
    if v < t.val(node)
        [t, done] = bst_remove_node(t, t.left(node), v, node, true);
    elseif v > t.val(node)
        [t, done] = bst_remove_node(t, t.right(node), v, node, false);
    else
        done = true;
        replacement = t.right(node);
        if is_left
            t.left(parent) = replacement;
        else
            t.right(parent) = replacement;
        end
    end
end

function [idx, out] = bst_inorder(t, node, out, idx)
    if node == 0, return; end
    [idx, out] = bst_inorder(t, t.left(node), out, idx);
    out(idx) = t.val(node); idx = idx + 1;
    [idx, out] = bst_inorder(t, t.right(node), out, idx);
end

function [idx, out] = bst_preorder(t, node, out, idx)
    if node == 0, return; end
    out(idx) = t.val(node); idx = idx + 1;
    [idx, out] = bst_preorder(t, t.left(node), out, idx);
    [idx, out] = bst_preorder(t, t.right(node), out, idx);
end

function [idx, out] = bst_postorder(t, node, out, idx)
    if node == 0, return; end
    [idx, out] = bst_postorder(t, t.left(node), out, idx);
    [idx, out] = bst_postorder(t, t.right(node), out, idx);
    out(idx) = t.val(node); idx = idx + 1;
end
