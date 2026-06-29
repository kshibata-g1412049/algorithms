function sorted = selection_sort(arr)
% SELECTION_SORT 配列を昇順にソートする（選択ソート）
%
%   未確定区間の中から最小値を探し、それを未確定区間の先頭要素と交換する、
%   という操作を繰り返すことで配列の先頭から順にソート済み区間を
%   拡大していくアルゴリズム。交換回数が最大でも n-1 回に抑えられる点が特徴。
%
%   時間複雑度: 最良/平均/最悪 すべて O(n^2)
%   空間複雑度: O(n)（入力を変更せず新しい配列を返すため）
%
%   sorted = SELECTION_SORT(arr) は、arr を昇順にソートした新しい配列を返す。
%   入力 arr は変更されない。

    sorted = arr; % 入力を破壊しないようにコピーを作成
    n = length(sorted);

    for i = 1:(n - 1)
        minIndex = i; % 未確定区間 [i, n] の中で最小値を持つ要素の添字

        % 未確定区間全体を走査して最小値の位置を探す
        for j = (i + 1):n
            if sorted(j) < sorted(minIndex)
                minIndex = j;
            end
        end

        % 最小値を未確定区間の先頭（i番目）に移動する
        if minIndex ~= i
            temp = sorted(i);
            sorted(i) = sorted(minIndex);
            sorted(minIndex) = temp;
        end
    end
end
