function idx = ternary_search(arr, target)
% TERNARY_SEARCH ソート済み配列を三分探索でtargetを探す
%
%   探索範囲を3等分する2つの点mid1, mid2を取り、targetとの比較によって
%   3つの部分区間のうち探索を続けるべき1つに絞り込む。二分探索が範囲を
%   1/2に絞るのに対し、三分探索は1/3に絞り込むが、1回の反復で2回の比較を
%   行うため、比較回数の観点では二分探索より効率が劣ることが多い
%   （定数倍の違い）。
%
%   前提条件: 入力arrは昇順にソート済みであること（事前条件の検証は行わない）。
%
%   時間複雑度: O(log3 n)
%   空間複雑度: O(1)
%
%   idx = TERNARY_SEARCH(arr, target) は、targetが見つかった場合は
%   1始まりのインデックス（複数該当する場合はいずれかのインデックス）を、
%   見つからなければ-1を返す。

    low = 1;
    high = length(arr);

    while low <= high
        % 探索範囲を3等分する2つの分割点
        mid1 = low + floor((high - low) / 3);
        mid2 = high - floor((high - low) / 3);

        if arr(mid1) == target
            idx = mid1;
            return;
        end
        if arr(mid2) == target
            idx = mid2;
            return;
        end

        if target < arr(mid1)
            high = mid1 - 1; % 第1区間に絞り込む
        elseif target > arr(mid2)
            low = mid2 + 1; % 第3区間に絞り込む
        else
            low = mid1 + 1; % 第2区間（中央）に絞り込む
            high = mid2 - 1;
        end
    end

    idx = -1;
end
