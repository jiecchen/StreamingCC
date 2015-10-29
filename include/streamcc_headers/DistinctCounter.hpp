/**
   @file DistinctCounter.hpp
   @author Jiecao Chen <chenjiecao@gmail.com>

   This file provides a class template for counting
   number of distinct elements in a data stream.

   The internal algorithm being used is BJKST-Sketch,
   Paper: [Counting distinct elements in a data stream](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.12.6276&rep=rep1&type=pdf)
*/

#ifndef __DISTINCT_HPP__
#define __DISTINCT_HPP__
#include "Sketch.h"
#include "utils.hpp"
#include <stdint.h>
#include <map>




namespace SccAux {
  template <typename T>
  class BJKST_basic: public Scc::Sketch<T> {
  private:
    int buf_size;
    std::map<int, int> buf;
    uint32_t seed;
  public:
    BJKST_basic(int _buf_size //!< buffer size
		): buf_size(_buf_size) {
      seed
    }
    
    void processItem(const T &item, double weight=1.) {
      uint32_t h = 
    }
  };
}


namespace Scc {
  template <typename T>
  class DistinctCounter: public Sketch<T> {
    
  };
}
#endif











