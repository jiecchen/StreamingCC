/* Copyright (C) 2015 by Jiecao Chen (chenjiecao@gmail.com) */

#ifndef __SKETCH_H__
#define __SKETCH_H__

/* Abstract Class for Sketch
 *
 * This class defines several common
 * interfaces shared by all Sketches
 * in this library.
 */

namespace Scc {
  template <typename T>   
  class Sketch {
  public:
    //! process a given (possibly weighted) item
    virtual void processItem(const T &item, double weight) = 0;
  };
}

#endif
