/**
   @file DistinctCounter.hpp
   @author Jiecao Chen <chenjiecao@gmail.com>

   This file provides a class template for counting
   number of distinct elements in a data stream.

   The internal algorithm being used is BJKST-Sketch,
   Paper: [Counting distinct elements in a data stream](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.12.6276&rep=rep1&type=pdf)
*/

#ifndef __DISTINCT_HPP__
#define __DISTINCT_HPP__
#include "Sketch.h"
#include "utils.hpp"
#include "Hash.h"
#include <stdint.h>
#include <functional>
#include <utility> // for pair
#include <map>
#include <set>
#include <vector>



namespace SccAux {
  template <typename T>
  class BJKST_basic: public Scc::Sketch<T> {
  private:
    int buf_size;
    int z; // thresh
    std::set<std::pair<int, int>> buf;
    //    std::map<std::pair<int, int>, int> buf;
    uint32_t seed_h;
    uint32_t seed_g;
    std::hash<T> hash_fn; // to cast a T to int
  public:
    BJKST_basic(int _buf_size //!< buffer size
		): buf_size(_buf_size), z(0) {
      seed_h = utils::rand_int();
      seed_g = utils::rand_int();
    }
    
    void processItem(const T &item, double weight=1.) {      
      int j = hash_fn(item);
      uint32_t h = murmurhash(&j, seed_h); 
      int lzeros = utils::zeros(h);
      if (lzeros >= z) { 
	uint32_t g = murmurhash(&j, seed_g);
	buf.insert(std::make_pair(g, lzeros));
      }
      // shrank buffer
      while (buf.size() >= buf_size) {
	z++;
	auto buf_cp = buf;
	for (auto &p : buf_cp) {
	  if (p.second < z)
	    buf.erase(p);
	}
      }
    }

    //! return estimation of number of distinct elements
    int getEstDistinct() {
      return buf.size() * (1 << z);
    }
  };
}


// namespace Scc {
//   template <typename T>
//   class DistinctCounter: public Sketch<T> {
    
//   };
// }
#endif











