function idx = binary_search(arr, target)
% BINARY_SEARCH ソート済み配列を二分探索でtargetを探す
%
%   探索範囲の中央要素とtargetを比較し、targetが中央要素より小さければ
%   左半分、大きければ右半分に絞り込むことを繰り返す。比較を1回行うたびに
%   探索範囲が半分になるため、非常に高速に探索できる。
%
%   前提条件: 入力arrは昇順にソート済みであること（事前条件の検証は行わない）。
%
%   時間複雑度: O(log n)
%   空間複雑度: O(1)
%
%   idx = BINARY_SEARCH(arr, target) は、targetが見つかった場合は
%   1始まりのインデックス（複数該当する場合はいずれかのインデックス）を、
%   見つからなければ-1を返す。

    low = 1;
    high = length(arr);

    while low <= high
        mid = floor((low + high) / 2);

        if arr(mid) == target
            idx = mid;
            return;
        elseif arr(mid) < target
            low = mid + 1; % 右半分に絞り込む
        else
            high = mid - 1; % 左半分に絞り込む
        end
    end

    idx = -1;
end
