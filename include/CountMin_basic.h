#ifndef __COUNT_MIN_BASIC_H__
#define __COUNT_MIN_BASIC_H__
#include <vector>
#include "config.h"



typedef std::vector<ItemType> Buffer;


//! CountMin_basic only process ItemType (aka int) stream
class CountMin_basic {
private:
  int m; // size of each buffer
  int d; // # of copies of buffer
  std::vector<Buffer> d_buf; // to keep d buffers
  std::vector<int> seeds;
public:
  CountMin_basic(int _m, //!< size of buffer 
		 int _d=20 //!< # of buffers
		 );
  //! process a given (weighted) item
  void processItem(const ItemType &item, double weight=1);
  //! @return estimation of total weight of the given item
  double estTotWeight(const ItemType &item);
};

#endif
