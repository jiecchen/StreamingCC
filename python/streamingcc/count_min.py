import streamingcc._count_min as _count_min

CountMinInt = _count_min.PyCountMinInt

# TODO(jiecchen): make sure `hash` is not misused.
class CountMin:
    """
    Count-Min sketch that handles all types.
    """
    def __init__(self, bucket_size, num_copies):
        self._cm_int = CountMinInt(bucket_size, num_copies)

    def process_item(self, item, weight):
        self._cm_int.process_item(hash(item) & 0xffffffff, weight)

    def get_estimation(self, item):
        """
        Returns the estimated frequency of the given item.
        """
        return self._cm_int.get_estimation(hash(item) & 0xffffffff)
