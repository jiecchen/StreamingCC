/**
   @file F2.hpp
   @author Jiecao Chen <chenjiecao@gmail.com>
   
   This file implements the class template that estimate the 2nd moment of a vector
   (i.e. \f$\|\textbf{x}\|_2^2\f$). It is also a linear sketch.

   Paper: [The space complexity of approximating the frequency moments](http://www.tau.ac.il/~nogaa/PDFS/amsz4.pdf)
*/



#ifndef __F2_HPP__
#define __F2_HPP__
#include "Sketch.hpp"
#include "utils.hpp"
#include <vector>
#include <functional>
#include <algorithm>
#include "Hash.h"
#include <stdint.h>

namespace SccAux {
  //! Basic class for F2 estimator, internal use only
  template <typename T>
  class F2_basic: Scc::Sketch<T> {
  private:
    std::vector<uint32_t> seeds;
    std::vector<double> counters;
    std::hash<T> hash_fn;
  public:
    
    //! Constructor
    F2_basic(int _buf_size //!< size of buffer
	     ) {
      // set random seeds
      for (int i = 0; i < _buf_size; ++i) {
	seeds.push_back(utils::rand_int());
	counters.push_back(0);
      }      
    }
    
    void processItem(const T &item, double weight=1.) {
      int h = hash_fn(item);
      for (size_t i = 0; i < seeds.size(); ++i) {
	int sign = (murmurhash(&h, seeds[i]) % 2) * 2 - 1;
	counters[i] += sign * weight;
      }
    }

    //! get estimated 2nd moment
    double getEstF2() {
      double tot = 0.;
      for (auto x: counters)
	tot += x * x;
      return tot / counters.size();
    }
  };


}

namespace Scc {
  //! Class for estimating the 2nd frequency moment of a weighted data stream
  /**
     The 2nd moment of a vector \f$\textbf{x}\f$ is defined as \f$\|\textbf{x}\|_2^2\f$.
     The 2nd moment of frequency vector of a data stream can be used to mearsure
     how skew the data is.
     
     This class works for weighted data stream as well (i.e. you can assign a weight for each item).
     When weight is omitted, \f$1\f$ is used by default.
     
     The relative error of estimation is bounded by \f$\frac{1}{\sqrt{buf\_size}}\f$.
     Success probability is at least \f$1 - 2^{-d}\f$.
  */
  template <typename T>
  class F2: Sketch<T> {  
  private:
    std::vector<SccAux::F2_basic<T>> estimators;
  public:
    //! Constructor
    F2(int buf_size, //!< size of buffer
       int d=20 //!< number of independent copies
       ) {
      for (int i = 0; i < d; ++i)
	estimators.push_back(SccAux::F2_basic<T>(buf_size));
    }
    
    void processItem(const T &item, double weight=1.) {
      for (auto &est: estimators) {
	est.processItem(item, weight);
      }
    }
    
    //! get estimated 2nd moment
    double getEstF2() {
      auto d = estimators.size();
      double est[d];
      for (size_t i = 0; i < d; ++i)
	est[i] = estimators[i].getEstF2();
      std::nth_element(est, est + d / 2, est + d);
      return est[d / 2];
    }
  };

  /**
     @example F2_Example.cpp
  */

}

#endif
















