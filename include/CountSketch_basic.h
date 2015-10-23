// Copyright (C) 2015 by Jiecao Chen (chenjiecao@gmail.com)

#ifndef __COUNT_SKETCH_BASIC_H__
#define __COUNT_SKETCH_BASIC_H__
#include <vector>
#include "config.h"
#include "Sketch.h"


typedef std::vector<ItemType> Buffer;


//! CountSketch_basic only process ItemType (aka int) stream, weight can be negative
class CountSketch_basic: public Sketch<ItemType> {
private:
  int m; // size of each buffer
  int d; // # of copies of buffer
  std::vector<Buffer> d_buf; // to keep d buffers
  std::vector<int> seeds;
  std::vector<int> sign_seeds; // seeds to generate random sign
public:
  CountSketch_basic(int _m, //!< size of buffer 
		 int _d=20 //!< number of buffers
		 );
  //! process a given (weighted) item
  void processItem(const ItemType &item, double weight=1);
  //! @return estimation of total weight of the given item
  double estTotWeight(const ItemType &item);
};

#endif









