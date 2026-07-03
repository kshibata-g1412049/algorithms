from bloom_filter import BloomFilter


def test_no_false_negatives():
    bf = BloomFilter(4096, 4)
    for i in range(100):
        bf.insert(i)
    assert all(bf.contains(i) for i in range(100))


def test_false_positives_in_tiny_filter():
    bf = BloomFilter(8, 3)
    for i in range(20):
        bf.insert(i)
    fp = sum(1 for i in range(1000, 1100) if bf.contains(i))
    assert fp > 0
