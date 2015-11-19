/**
    @file Sketch.h
    @author Jiecao Chen (chenjiecao@gmail.com)

    This file defines an abstract class for Sketches
*/

#ifndef __SKETCH_H__
#define __SKETCH_H__

//! Abstract Class for Sketch
/**
 * This class defines several common
 * interfaces shared by all Sketches
 * in this library.
 */

namespace Scc {
    template<typename T>
    class Sketch {
    public:
        //! process a given (possibly weighted) item
        virtual void processItem(const T &item, double weight) = 0;
    };
}

#endif
