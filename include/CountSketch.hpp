#ifndef __COUNT_SKETCH_H__
#define __COUNT_SKETCH_H__
#include "Sketch.h"
#include "CountSketch_basic.h"
#include <functional>
#include <iostream>

//! Process general type of data stream
template <typename T>
class CountSketch: public Sketch<T> {
private:
  CountSketch_basic csb;
  std::hash<T> hash_fn; // to cast a T to int
public:
  CountSketch(int _m, //!< size of buffer 
	   int _d=20 //!< number of buffers
	   ): csb(_m, _d) {};

  //! process a given (weighted) item
  void processItem(const T &item, double weight=1) {
    csb.processItem(hash_fn(item), weight);
  }

  //! give an estimation of total weight of the given item
  double estTotWeight(const T &item) {
    // convert item to ItemType using hash_fn
    //    std::cerr << item << " -> " << hash_fn(item) << std::endl;
    return csb.estTotWeight(hash_fn(item));
  }
  
};

#endif


















