/* Copyright (C) 2015 by Jiecao Chen (chenjiecao@gmail.com) */

#ifndef __SAMPLING_HPP__
#define  __SAMPLING_HPP__
#include "Sketch.h"
#include <vector>
#include "utils.h"
#include <cmath>

template <typename T>
class Sampling {
public:
  virtual void processItem(const T &item, double weight) = 0;
  virtual std::vector<T> getSamples() = 0;
}; 


//! Uniform sampling over a data stream
/*!
 * Internally, it's using the notable `reservoir sampling`
 * algorithm. Sampling can be done with or without replacement.
 */
template <typename T>
class UniformSampling: public Sampling<T> {
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
	if (rand_double() < t)
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
      int p = rand_int((int) totWeight);
      if (p < nSamples)
	samples[p] = item;
    }    
  };

public:
  UniformSampling(int _nSamples, //!< number of samples wanted
		  bool _withRpl=true  //!< whether sampling with replacement	 
		  ): 
    nSamples(_nSamples), withReplacementQ(_withRpl), totWeight(0.) {};
  
  //! process a given item
  /*!
    Note: for now `without_replacement` version
    can only handle unweighted data stream
  */
  void processItem(const T &item, 
		   double weight=1. //!< weight must be positive
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


#endif














