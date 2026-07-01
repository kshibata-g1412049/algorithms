function idx = exponential_search(arr, target)
% EXPONENTIAL_SEARCH ソート済み配列を指数探索でtargetを探す
%
%   探索範囲を1, 2, 4, 8, ... と倍々に広げていき、target以上の値を持つ
%   要素が現れた区間（あるいは配列の終端）を特定する。その後、特定した
%   区間に対して二分探索を行う。範囲が分かっている配列やtargetが先頭に
%   近い場合に有効で、特に上限が不明な（無限に続く）配列に対する探索にも
%   応用できる。
%
%   前提条件: 入力arrは昇順にソート済みであること（事前条件の検証は行わない）。
%
%   時間複雑度: O(log n)
%   空間複雑度: O(1)
%
%   idx = EXPONENTIAL_SEARCH(arr, target) は、targetが見つかった場合は
%   1始まりのインデックス（複数該当する場合はいずれかのインデックス）を、
%   見つからなければ-1を返す。

    n = length(arr);
    if n == 0
        idx = -1;
        return;
    end

    if arr(1) == target
        idx = 1;
        return;
    end

    % 探索範囲を1, 2, 4, 8, ... と倍々に広げ、target以上の値を持つ要素が
    % 現れる区間（または配列の終端）を特定する
    bound = 1;
    while bound < n && arr(bound + 1) < target
        bound = bound * 2;
    end

    % 特定した区間 [bound/2 + 1, min(bound + 1, n)] に対して二分探索を行う
    % （MATLABは1始まりのため、0始まりの境界bound/bound*2に+1のオフセットが必要）
    low = floor(bound / 2) + 1;
    high = min(bound + 1, n);
    idx = local_binary_search(arr, low, high, target);
end

function idx = local_binary_search(arr, low, high, target)
% [low, high] の範囲に対する標準的な二分探索（内部使用）
    idx = -1;
    while low <= high
        mid = floor((low + high) / 2);
        if arr(mid) == target
            idx = mid;
            return;
        elseif arr(mid) < target
            low = mid + 1;
        else
            high = mid - 1;
        end
    end
end
