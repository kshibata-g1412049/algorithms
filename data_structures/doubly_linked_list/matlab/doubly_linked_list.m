function varargout = doubly_linked_list(varargin)
%DOUBLY_LINKED_LIST  双方向連結リスト（並列配列＋整数インデックス）
%
%   ll = DOUBLY_LINKED_LIST('new')                   新しいリストを生成
%   ll = DOUBLY_LINKED_LIST(ll, 'push_front', v)     先頭に挿入
%   ll = DOUBLY_LINKED_LIST(ll, 'push_back',  v)     末尾に追加
%   [ll, v] = DOUBLY_LINKED_LIST(ll, 'pop_front')    先頭を取り出す
%   [ll, v] = DOUBLY_LINKED_LIST(ll, 'pop_back')     末尾を取り出す
%   v = DOUBLY_LINKED_LIST(ll, 'front')              先頭値を参照
%   v = DOUBLY_LINKED_LIST(ll, 'back')               末尾値を参照
%   b = DOUBLY_LINKED_LIST(ll, 'contains', v)        値を検索
%   [ll, b] = DOUBLY_LINKED_LIST(ll, 'remove', v)   最初の値を削除
%   n = DOUBLY_LINKED_LIST(ll, 'size')               要素数
%   b = DOUBLY_LINKED_LIST(ll, 'is_empty')           空かどうか
%   a = DOUBLY_LINKED_LIST(ll, 'to_array')           配列に変換

    MAX = 1024;

    if nargin == 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        ll = struct('val',  zeros(1, MAX), ...
                    'prv',  zeros(1, MAX), ...
                    'nxt',  zeros(1, MAX), ...
                    'head', 0, ...
                    'tail', 0, ...
                    'free', 1, ...
                    'sz',   0);
        varargout{1} = ll;
        return;
    end

    ll = varargin{1};
    op = varargin{2};

    switch op
        case 'push_front'
            v   = varargin{3};
            idx = ll.free; ll.free = ll.free + 1;
            ll.val(idx) = v;
            ll.prv(idx) = 0;
            ll.nxt(idx) = ll.head;
            if ll.head ~= 0
                ll.prv(ll.head) = idx;
            else
                ll.tail = idx;
            end
            ll.head = idx;
            ll.sz = ll.sz + 1;
            varargout{1} = ll;

        case 'push_back'
            v   = varargin{3};
            idx = ll.free; ll.free = ll.free + 1;
            ll.val(idx) = v;
            ll.nxt(idx) = 0;
            ll.prv(idx) = ll.tail;
            if ll.tail ~= 0
                ll.nxt(ll.tail) = idx;
            else
                ll.head = idx;
            end
            ll.tail = idx;
            ll.sz = ll.sz + 1;
            varargout{1} = ll;

        case 'pop_front'
            if ll.head == 0
                error('doubly_linked_list:underflow', 'list is empty');
            end
            v = ll.val(ll.head);
            nxt = ll.nxt(ll.head);
            if nxt ~= 0
                ll.prv(nxt) = 0;
            else
                ll.tail = 0;
            end
            ll.head = nxt;
            ll.sz = ll.sz - 1;
            varargout{1} = ll;
            varargout{2} = v;

        case 'pop_back'
            if ll.tail == 0
                error('doubly_linked_list:underflow', 'list is empty');
            end
            v = ll.val(ll.tail);
            prv = ll.prv(ll.tail);
            if prv ~= 0
                ll.nxt(prv) = 0;
            else
                ll.head = 0;
            end
            ll.tail = prv;
            ll.sz = ll.sz - 1;
            varargout{1} = ll;
            varargout{2} = v;

        case 'front'
            if ll.head == 0
                error('doubly_linked_list:underflow', 'list is empty');
            end
            varargout{1} = ll.val(ll.head);

        case 'back'
            if ll.tail == 0
                error('doubly_linked_list:underflow', 'list is empty');
            end
            varargout{1} = ll.val(ll.tail);

        case 'contains'
            v = varargin{3};
            cur = ll.head;
            found = false;
            while cur ~= 0
                if ll.val(cur) == v, found = true; break; end
                cur = ll.nxt(cur);
            end
            varargout{1} = found;

        case 'remove'
            v = varargin{3};
            cur = ll.head;
            removed = false;
            while cur ~= 0
                if ll.val(cur) == v
                    p = ll.prv(cur);
                    n = ll.nxt(cur);
                    if p ~= 0, ll.nxt(p) = n; else ll.head = n; end
                    if n ~= 0, ll.prv(n) = p; else ll.tail = p; end
                    ll.sz = ll.sz - 1;
                    removed = true;
                    break;
                end
                cur = ll.nxt(cur);
            end
            varargout{1} = ll;
            varargout{2} = removed;

        case 'size'
            varargout{1} = ll.sz;

        case 'is_empty'
            varargout{1} = (ll.sz == 0);

        case 'to_array'
            a = zeros(1, ll.sz);
            cur = ll.head;
            i = 1;
            while cur ~= 0
                a(i) = ll.val(cur);
                i = i + 1;
                cur = ll.nxt(cur);
            end
            varargout{1} = a;

        otherwise
            error('doubly_linked_list:unknownOp', 'Unknown operation: %s', op);
    end
end
