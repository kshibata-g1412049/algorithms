#ifndef UNION_FIND_H
#define UNION_FIND_H

// 素集合データ構造 (Union-Find)
//
// 操作:
//   find(x)           : 代表元を返す（経路圧縮）  O(α(n))
//   unite(x, y)       : x と y の集合を合併（ランク結合）  O(α(n))
//   connected(x, y)   : 同じ集合かどうか  O(α(n))
//   size()            : 要素数（頂点数）  O(1)
//   components()      : 連結成分数  O(1)
//
// 頂点ID: 0～n-1。範囲外は std::out_of_range。

#include <stdexcept>
#include <vector>

class UnionFind {
public:
    explicit UnionFind(int n);

    int find(int x);
    bool unite(int x, int y);
    bool connected(int x, int y);
    int size() const;
    int components() const;

private:
    std::vector<int> parent_;
    std::vector<int> rank_;
    int n_;
    int components_;

    void check(int x) const;
};

#endif // UNION_FIND_H
