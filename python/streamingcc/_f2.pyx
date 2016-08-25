# distutils: language = c++
# distutils: sources = ../src/f2.cc

cdef extern from "../../src/streamingcc_include/f2.h" namespace "streamingcc::integer":
    cdef cppclass F2Int:
        F2Int() except +
        F2Int(size_t, size_t) except +
        void ProcessItem(unsigned int, double)
        double GetEstimation()


cdef class PyF2Int:
    cdef F2Int f2int
    def __cint__(self, size_t bucket_size, size_t num_copies):
        self.f2int = F2Int(bucket_size, num_copies)
    def process_item(self, unsigned int item, double weight):
        self.f2int.ProcessItem(item, weight)
    def get_estimation(self):
        return self.f2int.GetEstimation()

        
