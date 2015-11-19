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

#include "Sketch.hpp"
#include "utils.hpp"
#include "Hash.h"
#include <stdint.h>
#include <functional>
#include <utility> // for pair
#include <map>
#include <set>
#include <vector>
#include <algorithm>


namespace SccAux {
    //! BJKST-Sketch with constant success probability - internal use only
    template<typename T>
    class BJKST_basic : public Scc::Sketch<T> {
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
        ) : buf_size(_buf_size), z(0) {
            seed_h = utils::rand_int();
            seed_g = utils::rand_int();
        }

        void processItem(const T &item, double weight = 1.) {
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


namespace Scc {




    //! Class template to count number of distinct elements over a data stream
    /**
     * The relative error of the estimation is bounded by \f$\frac{1}{\sqrt{\text{buf_size}}}\f$.
     * Success probility is at least \f$1 - 2^{-d}\f$
     */

    template<typename T>
    class DistinctCounter : public Sketch<T> {
    private:
        std::vector<SccAux::BJKST_basic<T>> bjkst;
    public:
        //! Constructor
        DistinctCounter(int buf_size, //!< size of each buffer
                        int d = 20 //!< number of buffers, success prob is at least \f$(1 - 2^{-d})\f$
        ) {
            // create d independent BJKST sketches
            for (int i = 0; i < d; ++i)
                bjkst.push_back(SccAux::BJKST_basic<T>(buf_size));
        }

        void processItem(const T &item, double weight = 1.) {
            for (auto &s : bjkst)
                s.processItem(item);
        }

        //! return estimation of number of distinct elements
        int getEstDistinct() {
            int F0[bjkst.size()];
            for (size_t i = 0; i < bjkst.size(); ++i)
                F0[i] = bjkst[i].getEstDistinct();
            // get the median
            int d = bjkst.size();
            std::nth_element(F0, F0 + d / 2, F0 + d);
            return F0[d / 2];
        }
    };
    /**
       @example DistinctCounter_Example.cpp
    */

}

#endif











