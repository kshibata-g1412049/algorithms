function idx = interpolation_search(arr, target)
% INTERPOLATION_SEARCH ソート済み配列を補間探索でtargetを探す
%
%   二分探索が常に中央要素を見るのに対し、補間探索は探索範囲の両端の値と
%   targetの値の関係から「targetがありそうな位置」を線形補間によって推定し、
%   その位置から比較を始める。データが一様分布に近いほど効率が良くなる。
%
%   前提条件: 入力arrは昇順にソート済みであること（事前条件の検証は行わない）。
%
%   時間複雑度: 平均 O(log log n)（データが一様分布に近い場合）、最悪 O(n)
%   空間複雑度: O(1)
%
%   idx = INTERPOLATION_SEARCH(arr, target) は、targetが見つかった場合は
%   1始まりのインデックス（複数該当する場合はいずれかのインデックス）を、
%   見つからなければ-1を返す。

    low = 1;
    high = length(arr);

    while low <= high
        if low == high
            if arr(low) == target
                idx = low;
            else
                idx = -1;
            end
            return;
        end

        % 全要素が同じ値の場合、分母（arr(high)-arr(low)）が0になり
        % ゼロ除算が発生するため、その場合は線形探索的にlowを直接調べる
        if arr(high) == arr(low)
            if arr(low) == target
                idx = low;
            else
                idx = -1;
            end
            return;
        end

        % 両端の値とtargetの値から、targetがありそうな位置を線形補間で推定する
        pos = low + floor((high - low) * (target - arr(low)) / (arr(high) - arr(low)));

        % 推定位置が範囲外になる場合（targetが範囲外の値の場合）は探索失敗
        if pos < low || pos > high
            idx = -1;
            return;
        end

        if arr(pos) == target
            idx = pos;
            return;
        elseif arr(pos) < target
            low = pos + 1; % 推定位置より右側に絞り込む
        else
            high = pos - 1; % 推定位置より左側に絞り込む
        end
    end

    idx = -1;
end
