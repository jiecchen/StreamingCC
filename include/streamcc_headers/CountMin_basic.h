/**
 @file CountMin_basic.h
 @author Jiecao Chen <chenjiecao@gmail.com>

 Header file of class CountMin_basic, which can only handle integer stream
 with positive weights
*/

#ifndef __COUNT_MIN_BASIC_H__
#define __COUNT_MIN_BASIC_H__
#include <vector>
#include "Sketch.h"


namespace SccAux {

  //! Buffer of int
  typedef std::vector<int> Buffer;


  //! CountMin_basic only process integer stream, weight must be non-negative
  class CountMin_basic: public Scc::Sketch<int> {
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
    void processItem(const int &item, double weight=1);
    //! return estimation of total weight of the given item
    double estTotWeight(const int &item);
  };

}

#endif
