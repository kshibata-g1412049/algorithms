"""quick_sort関数のテスト（pytest）。

このファイルは同じディレクトリ内のquick_sort.pyのみに依存しているため、
python/フォルダを単独で取り出してもこのテストは実行できる。
"""
from quick_sort import quick_sort


def test_empty_array():
    """空配列は空配列のまま"""
    assert quick_sort([]) == []


def test_single_element():
    """要素1つの配列はそのまま"""
    assert quick_sort([42]) == [42]


def test_already_sorted():
    """既にソート済みの配列"""
    assert quick_sort([1, 2, 3, 4, 5]) == [1, 2, 3, 4, 5]


def test_reverse_sorted():
    """完全な逆順の配列"""
    assert quick_sort([5, 4, 3, 2, 1]) == [1, 2, 3, 4, 5]


def test_with_duplicates():
    """重複要素を含む配列"""
    assert quick_sort([3, 1, 2, 3, 1]) == [1, 1, 2, 3, 3]


def test_with_negative_numbers():
    """負数を含む配列"""
    assert quick_sort([-3, 5, -1, 0, 2, -8]) == [-8, -3, -1, 0, 2, 5]


def test_does_not_mutate_input():
    """元の配列が変更されないことを確認"""
    original = [3, 1, 2]
    quick_sort(original)
    assert original == [3, 1, 2]
