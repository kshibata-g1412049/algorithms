from hash_set import HashSet


def test_new_is_empty():
    s = HashSet()
    assert s.is_empty()
    assert s.size() == 0


def test_insert_contains():
    s = HashSet()
    s.insert(1)
    s.insert(2)
    s.insert(3)
    assert s.contains(1)
    assert not s.contains(5)
    assert s.size() == 3


def test_no_duplicate():
    s = HashSet()
    s.insert(1)
    s.insert(1)
    assert s.size() == 1


def test_remove():
    s = HashSet()
    s.insert(1)
    s.insert(2)
    s.insert(3)
    assert s.remove(2)
    assert not s.contains(2)
    assert s.contains(1)
    assert not s.remove(99)
    assert s.size() == 2


def test_100_elements():
    s = HashSet()
    for i in range(100):
        s.insert(i)
    assert all(s.contains(i) for i in range(100))


def test_string_elements():
    s = HashSet()
    s.insert("alpha")
    s.insert("beta")
    s.insert("alpha")
    assert s.size() == 2
    assert s.contains("beta")
    assert not s.contains("gamma")
    assert s.remove("alpha")
    assert not s.contains("alpha")
