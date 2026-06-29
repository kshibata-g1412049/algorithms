function idx = fibonacci_search(arr, target)
% FIBONACCI_SEARCH ソート済み配列をフィボナッチ探索でtargetを探す
%
%   フィボナッチ数列を使って配列を不均等な比率で分割していく探索法。
%   配列サイズn以上となる最小のフィボナッチ数F(k)を求め、F(k-1)とF(k-2)を
%   使って分割位置を決める。除算を使わずに加減算のみで分割位置を計算できる
%   ため、除算コストが高い環境（古いCPUなど）では二分探索よりも有利になる
%   場合がある。
%
%   前提条件: 入力arrは昇順にソート済みであること（事前条件の検証は行わない）。
%
%   時間複雑度: O(log n)
%   空間複雑度: O(1)
%
%   idx = FIBONACCI_SEARCH(arr, target) は、targetが見つかった場合は
%   1始まりのインデックス（複数該当する場合はいずれかのインデックス）を、
%   見つからなければ-1を返す。
%
%   内部のoffsetは0始まりの「まだ調べていない先頭側の要素数」を表す概念で、
%   arr(i)へのアクセス時にのみ+1のオフセットを加えて1始まりに変換している
%   （counting_sortと同様、MATLABのインデックスは1から始まるための調整）。

    n = length(arr);
    if n == 0
        idx = -1;
        return;
    end

    % n以上となる最小のフィボナッチ数列 fibM = fibMMm1 + fibMMm2 を求める
    fibMMm2 = 0; % F(k-2)
    fibMMm1 = 1; % F(k-1)
    fibM = fibMMm2 + fibMMm1; % F(k)

    while fibM < n
        fibMMm2 = fibMMm1;
        fibMMm1 = fibM;
        fibM = fibMMm2 + fibMMm1;
    end

    % offsetは0始まりの「まだ調べていない先頭側の要素数」を表す
    offset = -1;

    while fibM > 1
        % F(k-2)分だけ進んだ位置（0始まり）を比較対象にする
        % （配列範囲を超えないようclamp）
        i = min(offset + fibMMm2, n - 1);

        % arr(i+1)でアクセスする（0始まりのiを1始まりに変換）
        if arr(i + 1) < target
            % targetはiより右側にあるので、フィボナッチ数列を1つ分小さくする
            fibM = fibMMm1;
            fibMMm1 = fibMMm2;
            fibMMm2 = fibM - fibMMm1;
            offset = i;
        elseif arr(i + 1) > target
            % targetはiより左側にあるので、フィボナッチ数列を2つ分小さくする
            fibM = fibMMm2;
            fibMMm1 = fibMMm1 - fibMMm2;
            fibMMm2 = fibM - fibMMm1;
        else
            idx = i + 1; % 1始まりのインデックスに変換して返す
            return;
        end
    end

    % 残り1要素（offset+1の位置、0始まり）を確認する
    if fibMMm1 == 1 && (offset + 1) < n && arr(offset + 2) == target
        idx = offset + 2; % 1始まりのインデックスに変換して返す
        return;
    end

    idx = -1;
end
