# distutils: language = c++
# distutils: sources = ../src/count_min.cc

cdef extern from "../../src/streamingcc_include/count_min.h" namespace "streamingcc::integer":
    cdef cppclass CountMinInt:
        CountMinInt() except +
        CountMinInt(size_t, size_t) except +
        void ProcessItem(unsigned int, double)
        double GetEstimation(unsigned int)


cdef class PyCountMinInt:
    cdef CountMinInt count_min_int
    def __cint__(self, size_t bucket_size, size_t num_copies):
        self.count_min_int = CountMinInt(bucket_size, num_copies)
    def process_item(self, unsigned int item, double weight):
        self.count_min_int.ProcessItem(item, weight)
    def get_estimation(self, unsigned int item):
        return self.count_min_int.GetEstimation(item)
