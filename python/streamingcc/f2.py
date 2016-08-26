import streamingcc._f2 as _f2

F2Int = _f2.PyF2Int

# TODO(jiecchen): make sure `hash` is not misused.
class F2:
    """
    F2 sketch that handles all types.
    This sketch returns an estimation of self-join size.
    """
    def __init__(self, bucket_size, num_copies):
        self._f2_int = F2Int(bucket_size, num_copies)

    def process_item(self, item, weight):
        self._f2_int.process_item(hash(item) & 0xffffffff, weight)

    def get_estimation(self):
        return self._f2_int.get_estimation()
