"""interpolation_search関数のテスト（pytest）。

このファイルは同じディレクトリ内のinterpolation_search.pyのみに依存しているため、
python/フォルダを単独で取り出してもこのテストは実行できる。
"""
from interpolation_search import interpolation_search


def test_empty_array():
    """空配列は常に-1"""
    assert interpolation_search([], 5) == -1


def test_single_element_match():
    """要素1つの配列：一致する場合"""
    assert interpolation_search([42], 42) == 0


def test_single_element_no_match():
    """要素1つの配列：一致しない場合"""
    assert interpolation_search([42], 7) == -1


def test_target_at_head():
    """targetが先頭にある場合"""
    arr = [10, 20, 30, 40, 50, 60, 70, 80, 90]
    assert interpolation_search(arr, 10) == 0


def test_target_at_tail():
    """targetが末尾にある場合"""
    arr = [10, 20, 30, 40, 50, 60, 70, 80, 90]
    assert interpolation_search(arr, 90) == 8


def test_target_at_middle():
    """targetが中央にある場合"""
    arr = [10, 20, 30, 40, 50, 60, 70, 80, 90]
    assert interpolation_search(arr, 50) == 4


def test_target_not_found():
    """targetが存在しない場合（最小値より小さい/最大値より大きい/間に挟まれる）"""
    arr = [10, 20, 30, 40, 50, 60, 70, 80, 90]
    assert interpolation_search(arr, 5) == -1
    assert interpolation_search(arr, 100) == -1
    assert interpolation_search(arr, 45) == -1


def test_with_duplicates():
    """重複要素を含む配列：複数の有効な解があり得るので、返ったインデックスの
    値がtargetと一致するかを確認する"""
    arr = [2, 4, 4, 4, 6, 8, 10]
    idx = interpolation_search(arr, 4)
    assert 0 <= idx < len(arr)
    assert arr[idx] == 4


def test_all_elements_equal():
    """全要素が同じ値の配列（分母が0になるケースの確認）"""
    arr = [5, 5, 5, 5, 5]
    idx = interpolation_search(arr, 5)
    assert 0 <= idx < len(arr)
    assert arr[idx] == 5
    assert interpolation_search(arr, 9) == -1
