from skip_list import SkipList


def test_new_is_empty():
    sl = SkipList()
    assert sl.is_empty()
    assert sl.size() == 0


def test_insert_and_search():
    sl = SkipList(seed=42)
    sl.insert(3)
    sl.insert(1)
    sl.insert(4)
    sl.insert(1)  # duplicate
    sl.insert(5)
    assert sl.search(3)
    assert sl.search(5)
    assert not sl.search(2)
    assert sl.size() == 4


def test_remove():
    sl = SkipList(seed=42)
    sl.insert(3)
    sl.insert(1)
    sl.insert(4)
    sl.insert(5)
    assert sl.remove(3)
    assert not sl.search(3)
    assert sl.search(1)
    assert not sl.remove(99)
    assert sl.size() == 3


def test_100_elements():
    sl = SkipList(seed=7)
    for i in range(1, 101):
        sl.insert(i)
    assert all(sl.search(i) for i in range(1, 101))
    for i in range(1, 51):
        sl.remove(i)
    assert not any(sl.search(i) for i in range(1, 51))
    assert all(sl.search(i) for i in range(51, 101))


def test_string_elements():
    sl = SkipList(seed=42)
    for w in ["banana", "apple", "cherry"]:
        sl.insert(w)
    assert sl.search("apple")
    assert not sl.search("durian")
    assert sl.remove("banana")
    assert sl.size() == 2
