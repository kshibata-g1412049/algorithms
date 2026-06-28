function sorted = quick_sort(arr)
% QUICK_SORT 配列を昇順にソートする（クイックソート）
%
%   配列から基準値（ピボット）を選び、ピボットより小さい要素を左側、
%   大きい要素を右側に振り分ける「分割（partition）」を行い、左右の部分配列を
%   それぞれ再帰的にソートする分割統治法に基づくアルゴリズム。本実装では
%   Lomutoのパーティション方式を用い、区間の末尾要素をピボットとする。
%
%   時間複雑度: 最良/平均 O(n log n)、最悪 O(n^2)（ピボットの選び方が常に偏る場合）
%   空間複雑度: O(log n)（再帰呼び出しのスタック分）
%
%   sorted = QUICK_SORT(arr) は、arr を昇順にソートした新しい配列を返す。
%   入力 arr は変更されない。

    sorted = arr; % 入力を破壊しないようにコピーを作成
    sorted = local_quick_sort(sorted, 1, length(sorted));
end

function arr = local_quick_sort(arr, low, high)
% 区間 [low, high] を再帰的にクイックソートするローカル関数。
    if low >= high
        return; % 要素数が0または1なら既にソート済み
    end

    [arr, pivotIndex] = local_partition(arr, low, high);
    arr = local_quick_sort(arr, low, pivotIndex - 1);
    arr = local_quick_sort(arr, pivotIndex + 1, high);
end

function [arr, pivotIndex] = local_partition(arr, low, high)
% Lomutoパーティション方式: 区間 [low, high] の末尾要素をピボットとし、
% ピボットより小さい要素を左側に集める。最終的なピボットの位置を返す。
    pivot = arr(high);
    i = low - 1; % ピボットより小さい要素の末尾位置

    for j = low:(high - 1)
        if arr(j) < pivot
            i = i + 1;
            temp = arr(i);
            arr(i) = arr(j);
            arr(j) = temp;
        end
    end

    temp = arr(i + 1);
    arr(i + 1) = arr(high);
    arr(high) = temp; % ピボットを正しい位置に移動
    pivotIndex = i + 1;
end
