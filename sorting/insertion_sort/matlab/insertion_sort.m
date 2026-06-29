function sorted = insertion_sort(arr)
% INSERTION_SORT 配列を昇順にソートする（挿入ソート）
%
%   配列の先頭からソート済み区間を少しずつ拡大していくアルゴリズム。
%   未確定区間の先頭要素を取り出し、ソート済み区間の中で正しい位置まで
%   後方の要素をずらしながら挿入していく。トランプの手札を並べる動作に近い。
%
%   時間複雑度: 最良 O(n)（既にソート済みの場合）、平均/最悪 O(n^2)
%   空間複雑度: O(n)（入力を変更せず新しい配列を返すため）
%
%   sorted = INSERTION_SORT(arr) は、arr を昇順にソートした新しい配列を返す。
%   入力 arr は変更されない。

    sorted = arr; % 入力を破壊しないようにコピーを作成
    n = length(sorted);

    for i = 2:n
        key = sorted(i); % 挿入対象の値
        j = i;

        % ソート済み区間 [1, i-1] の中で key より大きい要素を後ろにずらす
        while j > 1 && sorted(j - 1) > key
            sorted(j) = sorted(j - 1);
            j = j - 1;
        end

        sorted(j) = key; % 正しい位置にkeyを挿入
    end
end
