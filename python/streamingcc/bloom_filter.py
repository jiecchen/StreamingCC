import streamingcc._bloom_filter as _bloom_filter

bloom_filter = _bloom_filter.Pybloom_filter


class BF:

    def __init__(self, c):
        self.bf = bloom_filter(c)

    def add(self, x):
        self.bf.Add(x)

    def has(self, x):
        return self.bf.Has(x)
