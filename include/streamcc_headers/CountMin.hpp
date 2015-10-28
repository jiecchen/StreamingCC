// Copyright (C) 2015 by Jiecao Chen (chenjiecao@gmail.com)

#ifndef __COUNT_MIN_H__
#define __COUNT_MIN_H__
#include "Sketch.h"
#include "CountMin_basic.h"
#include <functional>
#include <iostream>

namespace Scc {
  //! Process general type of data stream
  template <typename T>
  class CountMin: public Sketch<T> {
  private:
    CountMin_basic cmb;
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
      // convert item to ItemType using hash_fn
      //    std::cerr << item << " -> " << hash_fn(item) << std::endl;
      return cmb.estTotWeight(hash_fn(item));
    }
  
  };
} // end namespace
#endif


















