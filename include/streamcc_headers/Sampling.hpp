/**
 @file Sampling.hpp
 @author Jiecao Chen <chenjiecao@gmail.com>

 This file implements algorithms that support sampling from data streams
*/


#ifndef __SAMPLING_HPP__
#define  __SAMPLING_HPP__

#include "Sketch.hpp"
#include <vector>
#include "utils.hpp"
#include <cmath>

namespace Scc {

    //! Abstract class to support sampling over data stream
    template<typename T>
    class Sampling {
    public:
        //! Process a given (weighted) item
        virtual void processItem(const T &item, //!< item with type T
                                 double weight //!< probability being sampled proportional to weight
        ) = 0;

        //! return the set of samples
        virtual std::vector<T> getSamples() = 0;
    };


    //! Sampling over a data stream via Reservoir Sampling
    /**
     * - For sampling with replacement, both weighted an unweighted
     *   data streams are supported.
     * - For sampling without replacement, currently only support unweighted
     *   data stream.
     */

    template<typename T>
    class ReservoirSampling : public Sampling<T> {
    private:
        std::vector<T> samples;
        int nSamples;
        bool withReplacementQ;
        int totWeight;

        void processItem_withRpl(const T &item, double weight) {
            if (samples.size() == 0) {
                for (int i = 0; i < nSamples; ++i)
                    samples.push_back(item);
            }
            else {
                double t = weight / totWeight;
                for (auto &sp: samples)
                    if (utils::rand_double() < t)
                        sp = item;
            }
        };

        //! TODO: for now, without_replacement version
        //        can only handle non-weighted sampling
        void processItem_withoutRpl(const T &item) {
            if (samples.size() < nSamples) {
                samples.push_back(item);
            }
            else {
                int p = utils::rand_int((int) totWeight);
                if (p < nSamples)
                    samples[p] = item;
            }
        };

    public:
        //! Constructor
        ReservoirSampling(int _nSamples, //!< number of samples wanted
                          bool _withRpl = true  //!< whether sampling with replacement
        ) :
                nSamples(_nSamples), withReplacementQ(_withRpl), totWeight(0.) { };

        //! process a given item
        /*!
          Note: for now `without_replacement` version
          can only handle unweighted data stream
        */
        void processItem(const T &item, //!< item to be processed
                         double weight = 1. //!< weight must be positive
        ) {
            if (!withReplacementQ)
                totWeight += 1;
            else
                totWeight += std::abs(weight);

            if (withReplacementQ)
                processItem_withRpl(item, weight);
            else
                processItem_withoutRpl(item);
        }

        std::vector<T> getSamples() {
            return samples;
        }
    };

}

#endif














