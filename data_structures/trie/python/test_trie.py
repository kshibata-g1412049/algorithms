from trie import Trie


def test_new_size_zero():
    t = Trie()
    assert t.size() == 0


def test_insert_and_search():
    t = Trie()
    t.insert("apple")
    assert t.search("apple")
    assert not t.search("app")
    assert t.starts_with("app")
    assert not t.starts_with("ban")
    assert t.size() == 1


def test_shared_prefix():
    t = Trie()
    t.insert("apple")
    t.insert("app")
    assert t.search("app")
    assert t.search("apple")
    assert t.size() == 2
    assert t.remove("app")
    assert t.search("apple")
    assert not t.search("app")
    assert t.size() == 1


def test_multiple_words():
    t = Trie()
    t.insert("cat")
    t.insert("car")
    t.insert("card")
    assert t.starts_with("ca")
    assert t.starts_with("car")
    assert not t.starts_with("cb")
    assert not t.remove("cap")


def test_remove_absent():
    t = Trie()
    t.insert("hello")
    assert not t.remove("hell")
    assert t.size() == 1
