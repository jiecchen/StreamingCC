/**
   @file CountMin.hpp
   @author Jiecao Chen <chenjiecao@gmail.com>
   
   This file implements the class template for [CountMin-Sketch](https://en.wikipedia.org/wiki/Count%E2%80%93min_sketch)
*/


#ifndef __COUNT_MIN_H__
#define __COUNT_MIN_H__
#include "Sketch.hpp"
#include "CountMin_basic.h"
#include <functional>
#include <iostream>

namespace Scc {

  //! CountMin-Sketch can be used to estimate the frequencies of items
  /**
     Estimation error of a given item \f$a\f$ can be bounded
     by \f$\frac{n - f_a}{m}\f$ where \f$n\f$ is the length of
     the data stream, \f$f_a\f$ is the actual frequency of \f$a\f$,
     and \f$m\f$ is the size of buffer.

     Success probability is at least \f$1 - 2^{-d}\f$.
  */
  template <typename T>
  class CountMin: public Sketch<T> {
  private:
    SccAux::CountMin_basic cmb;
    std::hash<T> hash_fn; // to cast a T to int
  public:
    //! Constructor
    CountMin(int _m, //!< size of buffer 
	     int _d=20 //!< number of buffers
	     ): cmb(_m, _d) {};

    //! process a given (weighted) item
    void processItem(const T &item, double weight=1) {
      cmb.processItem(hash_fn(item), weight);
    }

    //! return estimation of total weight of the given item
    double estTotWeight(const T &item) {
      return cmb.estTotWeight(hash_fn(item));
    }
  
    /**
       @example CountMin_Example.cpp
    */

  };
} // end namespace
#endif


















