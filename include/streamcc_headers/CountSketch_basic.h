/**
 @file CountSketch_basic.h
 @author Jiecao Chen <chenjiecao@gmail.com>

 Header file of class `CountSketch_basic`,
 which can only handle integer stream
*/
 

#ifndef __COUNT_SKETCH_BASIC_H__
#define __COUNT_SKETCH_BASIC_H__
#include <vector>
#include "Sketch.hpp"


namespace SccAux {

  //! `Buffer` type
  typedef std::vector<int> Buffer;


  //! CountSketch_basic only process integer stream, weight can be negative
  class CountSketch_basic: public Scc::Sketch<int> {
  private:
    int m; // size of each buffer
    int d; // # of copies of buffer
    std::vector<Buffer> d_buf; // to keep d buffers
    std::vector<int> seeds;
    std::vector<int> sign_seeds; // seeds to generate random sign
  public:
    //!  Constructor
    CountSketch_basic(int _m, //!< size of buffer 
		      int _d=20 //!< number of buffers
		      );
    //! process a given (weighted) item
    void processItem(const int &item, double weight=1);
    //! return estimation of total weight of the given item
    double estTotWeight(const int &item);
  };
}

#endif









