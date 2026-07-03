function varargout = singly_linked_list(varargin)
%SINGLY_LINKED_LIST  単方向連結リスト（並列配列＋整数インデックス）
%
%   ll = SINGLY_LINKED_LIST('new')                新しいリストを生成
%   ll = SINGLY_LINKED_LIST(ll, 'push_front', v)  先頭に挿入
%   ll = SINGLY_LINKED_LIST(ll, 'push_back',  v)  末尾に追加
%   [ll, v] = SINGLY_LINKED_LIST(ll, 'pop_front') 先頭を取り出す
%   v = SINGLY_LINKED_LIST(ll, 'front')           先頭値を参照
%   v = SINGLY_LINKED_LIST(ll, 'back')            末尾値を参照
%   b = SINGLY_LINKED_LIST(ll, 'contains', v)     値を検索
%   [ll, b] = SINGLY_LINKED_LIST(ll, 'remove', v) 最初の値を削除
%   n = SINGLY_LINKED_LIST(ll, 'size')            要素数
%   b = SINGLY_LINKED_LIST(ll, 'is_empty')        空かどうか
%   a = SINGLY_LINKED_LIST(ll, 'to_array')        配列に変換
%
%   0 = null sentinel。ノードは val/next 並列配列で管理。

    MAX = 1024;

    if nargin == 1 && ischar(varargin{1}) && strcmp(varargin{1}, 'new')
        ll = struct('val',  zeros(1, MAX), ...
                    'next', zeros(1, MAX), ...
                    'head', 0, ...
                    'free', 1, ...
                    'sz',   0);
        varargout{1} = ll;
        return;
    end

    ll = varargin{1};
    op = varargin{2};

    switch op
        case 'push_front'
            v = varargin{3};
            idx = ll.free;
            ll.free = ll.free + 1;
            ll.val(idx)  = v;
            ll.next(idx) = ll.head;
            ll.head = idx;
            ll.sz   = ll.sz + 1;
            varargout{1} = ll;

        case 'push_back'
            v = varargin{3};
            idx = ll.free;
            ll.free = ll.free + 1;
            ll.val(idx)  = v;
            ll.next(idx) = 0;
            if ll.head == 0
                ll.head = idx;
            else
                cur = ll.head;
                while ll.next(cur) ~= 0
                    cur = ll.next(cur);
                end
                ll.next(cur) = idx;
            end
            ll.sz = ll.sz + 1;
            varargout{1} = ll;

        case 'pop_front'
            if ll.head == 0
                error('singly_linked_list:underflow', 'list is empty');
            end
            v = ll.val(ll.head);
            ll.head = ll.next(ll.head);
            ll.sz = ll.sz - 1;
            varargout{1} = ll;
            varargout{2} = v;

        case 'front'
            if ll.head == 0
                error('singly_linked_list:underflow', 'list is empty');
            end
            varargout{1} = ll.val(ll.head);

        case 'back'
            if ll.head == 0
                error('singly_linked_list:underflow', 'list is empty');
            end
            cur = ll.head;
            while ll.next(cur) ~= 0
                cur = ll.next(cur);
            end
            varargout{1} = ll.val(cur);

        case 'contains'
            v = varargin{3};
            cur = ll.head;
            found = false;
            while cur ~= 0
                if ll.val(cur) == v
                    found = true;
                    break;
                end
                cur = ll.next(cur);
            end
            varargout{1} = found;

        case 'remove'
            v = varargin{3};
            removed = false;
            if ll.head ~= 0 && ll.val(ll.head) == v
                ll.head = ll.next(ll.head);
                ll.sz = ll.sz - 1;
                removed = true;
            elseif ll.head ~= 0
                cur = ll.head;
                while ll.next(cur) ~= 0
                    if ll.val(ll.next(cur)) == v
                        ll.next(cur) = ll.next(ll.next(cur));
                        ll.sz = ll.sz - 1;
                        removed = true;
                        break;
                    end
                    cur = ll.next(cur);
                end
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
                cur = ll.next(cur);
            end
            varargout{1} = a;

        otherwise
            error('singly_linked_list:unknownOp', 'Unknown operation: %s', op);
    end
end
