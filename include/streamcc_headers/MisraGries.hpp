/**
   @file MisraGries.hpp
   @author Jiecao Chen (chenjiecao@gmail.com)
   
   This file implements the class template for Misra-Gries algorithm.
*/

#ifndef __MISRA_GRIES_HPP__
#define __MISRA_GRIES_HPP__

#include "Sketch.h"
#include <map>
#include <vector>

namespace Scc {
  //! Class template for Misra-Gries algorithm

  /**
     Misra-Gries algorithm with parameter k (buffer size)
     can be used to find heavy hitters whose frequencies are
     larger than n/k, here n is the total number of items.

     For any item, Misra-Gries also gives an estimation of
     its frequency with error bounded by n/k.
  */
  template <typename T>
  class MisraGries: public Sketch<T> {
  private:
    int k; //!< buffer size
    std::map<T, int> buffer;
  public:
    //! Constructor
    MisraGries(int _k //!< buffer size
	       ): k(_k) {}

    void processItem(const T &item, double weight=1.) {
      if (buffer.find(item) != buffer.end()) { // if item in buffer
	buffer[item]++;
      }
      else if (buffer.size() < k) {
	buffer[item] = 1;
      }
      else {
	std::vector<T> toErase;
	// collect keys to be remove
	for (auto &p : buffer) {	  
	  p.second--;
	  if (p.second == 0)
	    toErase.push_back(p.first);
	}
	// remove keys from buffer
	for (auto &itm : toErase)
	  buffer.erase(itm);
      }
    }

    //! estimate the frequency of a given item
    int estFrequency(const T &item) {
      if (buffer.find(item) != buffer.end())
	return buffer[item];
      else
	return 0;
    }           

    //! return a std::map represents the top-k frequent items
    /**
       The keys of the returned map are the top-k items,
       while the values are their corresponding frequencies
    */
    std::map<T, int> getTopK() {
      return buffer;
    }

  };
  
}

#endif










