# distutils: language = c++
# distutils: sources = ../src/hyper_loglog.cc

from libcpp cimport bool

cdef extern from "../../src/streamingcc_include/hyper_loglog.h" namespace "streamingcc":
    cdef cppclass hyper_loglog:
        hyper_loglog() except +
        void update(unsigned int)
        double estimate()

cdef class Pyhyper_loglog:
    cdef hyper_loglog hyperloglog
    def __cint__(self):
        self.hyperloglog = hyper_loglog()
    def Update(self, unsigned int x):
        self.hyperloglog.update(x)
    def Estimate(self):
        return self.hyperloglog.estimate()


    
