function sorted = counting_sort(arr)
% COUNTING_SORT 配列（非負整数のみ）を昇順にソートする（計数ソート）
%
%   比較によらないソートアルゴリズム。各値が何回出現するかを数え上げる
%   カウント配列を作り、その累積和から各値を配置すべき位置を求めて
%   出力配列を構築する。値の範囲kが小さいほど高速になる。
%
%   制約: 本実装は非負整数（0以上の整数）のみを対象とする。
%
%   時間複雑度: O(n + k)（nは要素数、kは配列中の最大値）
%   空間複雑度: O(n + k)（カウント配列と出力用の配列を必要とする）
%
%   sorted = COUNTING_SORT(arr) は、arr を昇順にソートした新しい配列を返す。
%   入力 arr は変更されない。

    if isempty(arr)
        sorted = arr;
        return;
    end

    % 配列中の最大値を求め、カウント配列のサイズを決める
    % count(v+1) が値vの出現回数（MATLABは1始まりのため+1のオフセットが必要）
    maxValue = max(arr);
    count = zeros(1, maxValue + 1);

    % 各値の出現回数を数え上げる
    for i = 1:length(arr)
        count(arr(i) + 1) = count(arr(i) + 1) + 1;
    end

    % 累積和を取ることで、各値が出力配列の何番目までの位置に収まるかを求める
    for i = 2:length(count)
        count(i) = count(i) + count(i - 1);
    end

    % 累積和を使って出力配列を構築する（後ろから走査することで安定性を保つ）
    sorted = zeros(1, length(arr));
    for i = length(arr):-1:1
        v = arr(i);
        sorted(count(v + 1)) = v;
        count(v + 1) = count(v + 1) - 1;
    end
end
