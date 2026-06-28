function sorted = merge_sort(arr)
% MERGE_SORT 配列を昇順にソートする（マージソート）
%
%   配列を再帰的に半分に分割し（分割: Divide）、それぞれを再帰的にソートした
%   うえで（統治: Conquer）、2つのソート済み配列を先頭から比較しながら
%   1つにまとめる（結合: Merge）という分割統治法に基づくアルゴリズム。
%
%   時間複雑度: 最良/平均/最悪 すべて O(n log n)
%   空間複雑度: O(n)（マージ時に作業用配列を必要とする）
%
%   sorted = MERGE_SORT(arr) は、arr を昇順にソートした新しい配列を返す。
%   入力 arr は変更されない。

    n = length(arr);
    if n <= 1
        sorted = arr; % 要素数が0または1なら既にソート済み
        return;
    end

    % 配列を半分に分割し、それぞれを再帰的にソートする
    mid = floor(n / 2);
    left = merge_sort(arr(1:mid));
    right = merge_sort(arr(mid + 1:end));

    sorted = local_merge(left, right);
end

function merged = local_merge(left, right)
% 2つのソート済み配列を1つのソート済み配列にまとめるローカル関数。
    merged = zeros(1, length(left) + length(right));
    i = 1; % leftの読み取り位置
    j = 1; % rightの読み取り位置
    k = 1; % mergedへの書き込み位置

    % 両配列の先頭同士を比較し、小さい方を順にmergedへ詰めていく
    while i <= length(left) && j <= length(right)
        if left(i) <= right(j)
            merged(k) = left(i);
            i = i + 1;
        else
            merged(k) = right(j);
            j = j + 1;
        end
        k = k + 1;
    end

    % 残った要素をそのまま詰める（どちらか一方は既に詰め終わっている）
    while i <= length(left)
        merged(k) = left(i);
        i = i + 1;
        k = k + 1;
    end
    while j <= length(right)
        merged(k) = right(j);
        j = j + 1;
        k = k + 1;
    end
end
