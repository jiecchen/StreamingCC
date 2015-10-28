/* Copyright (C) 2015 by Jiecao Chen (chenjiecao@gmail.com) */

#ifndef __COUNT_MIN_BASIC_H__
#define __COUNT_MIN_BASIC_H__
#include <vector>
#include "config.h"
#include "Sketch.h"


namespace Scc {

  //! Buffer of ItemType
  typedef std::vector<ItemType> Buffer;


  //! CountMin_basic only process ItemType (aka int) stream, weight must be non-negative
  class CountMin_basic: public Sketch<ItemType> {
  private:
    int m; // size of each buffer
    int d; // # of copies of buffer
    std::vector<Buffer> d_buf; // to keep d buffers
    std::vector<int> seeds;
  public:
    //! Constructor
    CountMin_basic(int _m, //!< size of buffer 
		   int _d=20 //!< number of buffers
		   );
    //! process a given (weighted) item
    void processItem(const ItemType &item, double weight=1);
    //! return estimation of total weight of the given item
    double estTotWeight(const ItemType &item);
  };

}

#endif
