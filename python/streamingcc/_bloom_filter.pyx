# distutils: language = c++
# distutils: sources = ../src/bloom_filter.cc

from libcpp cimport bool

cdef extern from "../../src/streamingcc_include/bloom_filter.h" namespace "streamingcc":
    cdef cppclass bloom_filter:
        bloom_filter() except +
        bloom_filter(int) except + 
        void add(int)
        bool has(int)

cdef class Pybloom_filter:
    cdef bloom_filter bloomfilter
    def __cint__(self, int c):
        self.bloomfilter = bloom_filter(c)
    def Add(self, int x):
        self.bloomfilter.add(x)
    def Has(self, int x):
        return self.bloomfilter.has(x)


    
