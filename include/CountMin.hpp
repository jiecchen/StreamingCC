#ifndef __COUNT_MIN_H__
#define __COUNT_MIN_H__
#include "Hash.hpp"
#include "Sketch.h"
#include "CountMin_basic.h"



//! Process general type of data stream
template <typename T>
class CountMin: public Sketch<T> {
private:
  CountMin_basic cmb;

public:
  CountMin(int _m, //!< size of buffer 
	   int _d=20 //!< # of buffers
	   ): cmb(_m, _d) {};

  //! process a given (weighted) item
  void processItem(const T &item, double weight=1) {
    // convert item to ItemType using murmurhash
    ItemType hash_item = murmurhash(&item);
    cmb.processItem(hash_item, weight);
  }

  //! @return estimation of total weight of the given item
  double estTotWeight(const T &item) {
    // convert item to ItemType using murmurhash
    ItemType hash_item = murmurhash(&item);
    return cmb.estTotWeight(hash_item);
  }
  
};

#endif


















