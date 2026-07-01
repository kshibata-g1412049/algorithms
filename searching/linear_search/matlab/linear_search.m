function idx = linear_search(arr, target)
% LINEAR_SEARCH 配列を先頭から順に走査してtargetを探す（線形探索）
%
%   配列の先頭から末尾まで要素を1つずつ順番に比較していき、targetと
%   一致する要素が見つかればそのインデックスを返す。配列がソートされている
%   必要はなく、任意の配列に適用できる。
%
%   時間複雑度: 最良 O(1)（先頭要素が一致する場合）、平均/最悪 O(n)
%   空間複雑度: O(1)
%
%   idx = LINEAR_SEARCH(arr, target) は、targetが見つかった場合は
%   1始まりのインデックス（複数該当する場合はいずれかのインデックス）を、
%   見つからなければ-1を返す。

    idx = -1;
    for i = 1:length(arr)
        if arr(i) == target
            idx = i;
            return;
        end
    end
end
