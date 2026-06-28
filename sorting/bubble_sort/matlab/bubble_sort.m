function sorted = bubble_sort(arr)
% BUBBLE_SORT 配列を昇順にソートする（バブルソート）
%
%   隣り合う要素を比較し、順序が逆であれば交換するという操作を配列の
%   先頭から末尾まで繰り返すことで、1回のパスごとに未確定の最大値を
%   末尾側に「泡（バブル）」のように浮かび上がらせていくアルゴリズム。
%
%   時間複雑度: 最良 O(n)（既にソート済みの場合は早期終了する）、
%               平均/最悪 O(n^2)
%   空間複雑度: O(n)（入力を変更せず新しい配列を返すため）
%
%   sorted = BUBBLE_SORT(arr) は、arr を昇順にソートした新しい配列を返す。
%   入力 arr は変更されない。

    sorted = arr; % 入力を破壊しないようにコピーを作成
    n = length(sorted);

    for i = 1:(n - 1)
        swapped = false; % このパスで交換が発生したかどうか

        % 未確定区間 [1, n-i] を1回スキャンし、隣接要素を比較・交換する
        for j = 1:(n - i)
            if sorted(j) > sorted(j + 1)
                temp = sorted(j);
                sorted(j) = sorted(j + 1);
                sorted(j + 1) = temp;
                swapped = true;
            end
        end

        % 交換が一度も起きなかった = 既にソート済みなので早期終了
        if ~swapped
            break;
        end
    end
end
