% test_bfs.m
%
% bfs関数のテストスクリプト（MATLAB/Octave両対応）。
% このフォルダを単独で取り出してそのまま実行できる（外部依存なし）。
%
% MATLABは1始まりインデックスのため、ノード番号もすべて1始まりとして扱う。
%
% 実行方法: MATLAB/Octave上でこのスクリプトを実行する。
%   >> test_bfs
% 全テストが成功すれば "All tests passed." が表示される。

% 単一ノードのグラフ（隣接リストが空のノード1つ）
adjacency = {[]};
assert(isequal(bfs(adjacency, 1), [1]));

% 線形グラフ（1-2-3-4が一直線につながっている）
adjacency = {[2], [1, 3], [2, 4], [3]};
assert(isequal(bfs(adjacency, 1), [1, 2, 3, 4]));

% 分岐のある木構造のグラフ
%   1 -- 2 -- 4
%   |    |
%   3    5
adjacency = {[2, 3], [1, 4, 5], [1], [2], [2]};
assert(isequal(bfs(adjacency, 1), [1, 2, 3, 4, 5]));

% サイクルを含むグラフ（1-2-3-4-1のループ）。無限ループしないことを確認する。
adjacency = {[2], [3], [1, 4], [3]};
assert(isequal(bfs(adjacency, 1), [1, 2, 3, 4]));

% 孤立ノードを含む非連結グラフ。startから到達できないノードは結果に含まれない。
adjacency = {[2], [1], []};
assert(isequal(bfs(adjacency, 1), [1, 2]));

% BFSとDFSで訪問順序が異なることを確認するための、複数の子を持つグラフ
%   1 -- 2 -- 5
%   |
%   3
%   |
%   4
adjacency = {[2, 3, 4], [1, 5], [1], [1], [2]};
assert(isequal(bfs(adjacency, 1), [1, 2, 3, 4, 5]));

disp('All tests passed.');
