function varargout = avl_tree(varargin)
%AVL_TREE  AVL木（自己平衡二分探索木、並列配列＋整数インデックス）
%
%   t = AVL_TREE('new')             新しいAVL木を生成
%   t = AVL_TREE(t, 'insert', v)   値を挿入（重複無視）
%   b = AVL_TREE(t, 'search', v)   値を検索
%   [t, b] = AVL_TREE(t, 'remove', v) 値を削除
%   v = AVL_TREE(t, 'min')         最小値
%   v = AVL_TREE(t, 'max')         最大値
%   h = AVL_TREE(t, 'height')      木の高さ
%   a = AVL_TREE(t, 'inorder')     中順走査
%   n = AVL_TREE(t, 'size')        要素数
%   b = AVL_TREE(t, 'is_empty')    空かどうか

    MAX = 1024;

    if nargin == 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        t = struct('val',   zeros(1, MAX), ...
                   'ht',    zeros(1, MAX), ...
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
            [t, new_root] = avl_insert(t, t.root, v);
            t.root = new_root;
            varargout{1} = t;

        case 'search'
            v = varargin{3};
            cur = t.root;
            found = false;
            while cur ~= 0
                if v == t.val(cur), found = true; break; end
                if v < t.val(cur), cur = t.left(cur);
                else,              cur = t.right(cur); end
            end
            varargout{1} = found;

        case 'remove'
            v = varargin{3};
            [t, new_root, removed] = avl_remove(t, t.root, v);
            t.root = new_root;
            if removed, t.sz = t.sz - 1; end
            varargout{1} = t;
            varargout{2} = removed;

        case 'min'
            if t.root == 0
                error('avl_tree:underflow', 'tree is empty');
            end
            cur = t.root;
            while t.left(cur) ~= 0, cur = t.left(cur); end
            varargout{1} = t.val(cur);

        case 'max'
            if t.root == 0
                error('avl_tree:underflow', 'tree is empty');
            end
            cur = t.root;
            while t.right(cur) ~= 0, cur = t.right(cur); end
            varargout{1} = t.val(cur);

        case 'height'
            varargout{1} = avl_ht(t, t.root);

        case 'inorder'
            out = zeros(1, t.sz);
            [~, out] = avl_inorder(t, t.root, out, 1);
            varargout{1} = out;

        case 'size'
            varargout{1} = t.sz;

        case 'is_empty'
            varargout{1} = (t.sz == 0);

        otherwise
            error('avl_tree:unknownOp', 'Unknown operation: %s', op);
    end
end

function h = avl_ht(t, node)
    if node == 0, h = 0; else, h = t.ht(node); end
end

function t = avl_update_ht(t, node)
    t.ht(node) = 1 + max(avl_ht(t, t.left(node)), avl_ht(t, t.right(node)));
end

function bf = avl_bf(t, node)
    bf = avl_ht(t, t.left(node)) - avl_ht(t, t.right(node));
end

function [t, root] = avl_rot_right(t, y)
    x = t.left(y);
    t.left(y)  = t.right(x);
    t.right(x) = y;
    t = avl_update_ht(t, y);
    t = avl_update_ht(t, x);
    root = x;
end

function [t, root] = avl_rot_left(t, x)
    y = t.right(x);
    t.right(x) = t.left(y);
    t.left(y)  = x;
    t = avl_update_ht(t, x);
    t = avl_update_ht(t, y);
    root = y;
end

function [t, root] = avl_balance(t, node)
    t = avl_update_ht(t, node);
    b = avl_bf(t, node);
    if b > 1
        if avl_bf(t, t.left(node)) < 0
            [t, t.left(node)] = avl_rot_left(t, t.left(node));
        end
        [t, root] = avl_rot_right(t, node);
    elseif b < -1
        if avl_bf(t, t.right(node)) > 0
            [t, t.right(node)] = avl_rot_right(t, t.right(node));
        end
        [t, root] = avl_rot_left(t, node);
    else
        root = node;
    end
end

function [t, node_idx] = avl_insert(t, node, v)
    if node == 0
        idx = t.free; t.free = t.free + 1;
        t.val(idx)   = v;
        t.ht(idx)    = 1;
        t.left(idx)  = 0;
        t.right(idx) = 0;
        t.sz = t.sz + 1;
        node_idx = idx;
    elseif v < t.val(node)
        [t, child] = avl_insert(t, t.left(node), v);
        t.left(node) = child;
        [t, node_idx] = avl_balance(t, node);
    elseif v > t.val(node)
        [t, child] = avl_insert(t, t.right(node), v);
        t.right(node) = child;
        [t, node_idx] = avl_balance(t, node);
    else
        node_idx = node;
    end
end

function [t, node_idx, removed] = avl_remove(t, node, v)
    removed = false;
    if node == 0, node_idx = 0; return; end
    if v < t.val(node)
        [t, child, removed] = avl_remove(t, t.left(node), v);
        t.left(node) = child;
        if removed
            [t, node_idx] = avl_balance(t, node);
        else
            node_idx = node;
        end
    elseif v > t.val(node)
        [t, child, removed] = avl_remove(t, t.right(node), v);
        t.right(node) = child;
        if removed
            [t, node_idx] = avl_balance(t, node);
        else
            node_idx = node;
        end
    else
        removed = true;
        if t.left(node) == 0 && t.right(node) == 0
            node_idx = 0;
        elseif t.left(node) == 0
            node_idx = t.right(node);
        elseif t.right(node) == 0
            node_idx = t.left(node);
        else
            succ = t.right(node);
            while t.left(succ) ~= 0, succ = t.left(succ); end
            t.val(node) = t.val(succ);
            [t, child, ~] = avl_remove(t, t.right(node), t.val(succ));
            t.right(node) = child;
            [t, node_idx] = avl_balance(t, node);
            % undo the sz decrement that will happen after return
            t.sz = t.sz + 1;
        end
    end
end

function [idx, out] = avl_inorder(t, node, out, idx)
    if node == 0, return; end
    [idx, out] = avl_inorder(t, t.left(node), out, idx);
    out(idx) = t.val(node); idx = idx + 1;
    [idx, out] = avl_inorder(t, t.right(node), out, idx);
end
