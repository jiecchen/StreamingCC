/**
 @file CountSketch.hpp
 @author Jiecao Chen <chenjiecao@gmail.com>

 This file provide the class template for Count-Sketch Algorithm.
*/


#ifndef __COUNT_SKETCH_H__
#define __COUNT_SKETCH_H__

#include "Sketch.hpp"
#include "CountSketch_basic.hpp"
#include <functional>
#include <iostream>

namespace Scc {

    //! Count-Sketch can be used to estimate the frequencies of items
    /**
       Estimation error of a given item \f$a\f$ can be bounded
       by \f$\frac{\|\mathbf{f}_{-a}\|_2}{\sqrt{m}}\f$ where
       \f$\mathbf{f}_{-a}\f$ is the frequency vector without \f$a\f$
       (i.e. drop coorinate of \f$a\f$ from the data stream frequency vector),
       \f$m\f$ is the size of buffer.

       Success probability is at least \f$1 - 2^{-d}\f$.
    */

    template<typename T>
    class CountSketch : public Sketch<T> {
    private:
        SccAux::CountSketch_basic csb;
        std::hash<T> hash_fn; // to cast a T to int
    public:
        //! Constructor
        CountSketch(int _m, //!< size of buffer
                    int _d = 20 //!< number of buffers
        ) : csb(_m, _d) { };

        //! process a given (weighted) item
        void processItem(const T &item, //!< item to be processed
                         double weight = 1 //! associated weight
        ) {
            csb.processItem(hash_fn(item), weight);
        }

        //! give an estimation of total weight of the given item
        double estTotWeight(const T &item //!< item to be processed
        ) {
            // convert item to ItemType using hash_fn
            //    std::cerr << item << " -> " << hash_fn(item) << std::endl;
            return csb.estTotWeight(hash_fn(item));
        }

        /**
           @example CountSketch_Example.cpp
        */
    };

}
#endif


















