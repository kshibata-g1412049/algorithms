function sorted = shell_sort(arr)
% SHELL_SORT 配列を昇順にソートする（シェルソート）
%
%   挿入ソートを拡張したアルゴリズム。最初は大きく離れた要素同士
%   （gap=n/2など）を比較・交換して大まかに整列させ、gapを徐々に小さくして
%   最終的にgap=1（通常の挿入ソート相当）まで繰り返すことで、要素の
%   移動距離を減らし挿入ソートより高速化を図る。本実装ではgapをn/2から
%   開始し毎回半分にする方式（Shellの原論文のgap列）を用いる。
%
%   時間複雑度: gap列に依存。本実装（gap=n/2,n/4,...,1）では平均/最悪 O(n^2)、
%               より工夫したgap列を用いるとO(n log^2 n)程度まで改善できる
%   空間複雑度: O(n)（入力を変更せず新しい配列を返すため）
%
%   sorted = SHELL_SORT(arr) は、arr を昇順にソートした新しい配列を返す。
%   入力 arr は変更されない。

    sorted = arr; % 入力を破壊しないようにコピーを作成
    n = length(sorted);
    gap = floor(n / 2);

    % gapをn/2から開始し、1になるまで半分にしていく
    while gap > 0
        % gap離れた要素同士で挿入ソートを行う
        for i = (gap + 1):n
            temp = sorted(i);
            j = i;

            % gap分離れた手前の要素と比較し、tempより大きければ後ろにずらす
            while j > gap && sorted(j - gap) > temp
                sorted(j) = sorted(j - gap);
                j = j - gap;
            end

            sorted(j) = temp;
        end

        gap = floor(gap / 2);
    end
end
