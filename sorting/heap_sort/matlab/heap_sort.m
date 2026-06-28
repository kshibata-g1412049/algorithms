function sorted = heap_sort(arr)
% HEAP_SORT 配列を昇順にソートする（ヒープソート）
%
%   配列全体を最大ヒープ（親が子より必ず大きい二分木）として構築したうえで、
%   ヒープの根（=配列中の最大値）を配列の末尾と交換し、ヒープの範囲を1つ
%   縮めて再度ヒープ条件を満たすように調整する、という操作を繰り返す
%   アルゴリズム。これにより配列の末尾から順に確定値が積み上がっていく。
%
%   時間複雑度: 最良/平均/最悪 すべて O(n log n)
%   空間複雑度: O(n)（入力を変更せず新しい配列を返すため。配列自体は二分木として扱う）
%
%   sorted = HEAP_SORT(arr) は、arr を昇順にソートした新しい配列を返す。
%   入力 arr は変更されない。
%
%   注: MATLABは1始まりの添字のため、ノードiの子はそれぞれ2*i, 2*i+1番目
%       （0始まりでの2*i+1, 2*i+2に相当）として扱う。

    sorted = arr; % 入力を破壊しないようにコピーを作成
    n = length(sorted);

    if n < 2
        return;
    end

    % 配列全体を最大ヒープに構築する（葉ではない最後の親から逆順に調整）
    for i = floor(n / 2):-1:1
        sorted = local_sift_down(sorted, n, i);
    end

    % ヒープの根（最大値）を末尾と交換し、ヒープ範囲を1つ縮めて再調整する
    for endIdx = n:-1:2
        temp = sorted(1);
        sorted(1) = sorted(endIdx);
        sorted(endIdx) = temp;
        sorted = local_sift_down(sorted, endIdx - 1, 1);
    end
end

function arr = local_sift_down(arr, heapSize, i)
% 添字iを根とする部分木がヒープ条件を満たすように、根を下方向に沈めて調整する
% ローカル関数。heapSizeはヒープとして扱う範囲（[1, heapSize]）を表す。
    largest = i;
    left = 2 * i;
    right = 2 * i + 1;

    if left <= heapSize && arr(left) > arr(largest)
        largest = left;
    end
    if right <= heapSize && arr(right) > arr(largest)
        largest = right;
    end

    if largest ~= i
        temp = arr(i);
        arr(i) = arr(largest);
        arr(largest) = temp;
        arr = local_sift_down(arr, heapSize, largest); % 交換した先を再帰的に確認
    end
end
