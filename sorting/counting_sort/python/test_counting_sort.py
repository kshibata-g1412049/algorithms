"""counting_sort関数のテスト（pytest）。

このファイルは同じディレクトリ内のcounting_sort.pyのみに依存しているため、
python/フォルダを単独で取り出してもこのテストは実行できる。

counting_sortは非負整数のみを対象とするため、負数のテストケースは含めない。
"""
from counting_sort import counting_sort


def test_empty_array():
    """空配列は空配列のまま"""
    assert counting_sort([]) == []


def test_single_element():
    """要素1つの配列はそのまま"""
    assert counting_sort([42]) == [42]


def test_already_sorted():
    """既にソート済みの配列"""
    assert counting_sort([1, 2, 3, 4, 5]) == [1, 2, 3, 4, 5]


def test_reverse_sorted():
    """完全な逆順の配列"""
    assert counting_sort([5, 4, 3, 2, 1]) == [1, 2, 3, 4, 5]


def test_with_duplicates():
    """重複要素を含む配列"""
    assert counting_sort([3, 1, 2, 3, 1]) == [1, 1, 2, 3, 3]


def test_with_zeros():
    """0を含む配列"""
    assert counting_sort([0, 4, 0, 2, 1, 0]) == [0, 0, 0, 1, 2, 4]


def test_does_not_mutate_input():
    """元の配列が変更されないことを確認"""
    original = [3, 1, 2]
    counting_sort(original)
    assert original == [3, 1, 2]
