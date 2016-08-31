// Copyright 2016 Jiecao Chen
#ifndef SRC_STREAMINGCC_INCLUDE_COUNT_MIN_H_
#define SRC_STREAMINGCC_INCLUDE_COUNT_MIN_H_

#include <vector>
#include <cstdlib>
#include <memory>
#include <cassert>
#include <iostream>

#if (_LIBCPP_VERSION)
  #include <tr1/cstdint>
  #include <tr1/functional>
#else
  #include <functional>
  #include <cstdint>
#endif

#include "../streamingcc_include/streaming_algorithm.h"
#include "../streamingcc_include/util.h"

namespace streamingcc {

namespace integer {

// Basic version of Count-Min sketch, without aggregation.
class CountMinBasicInt: public StreamingAlgorithmWeightedInt {
 public:
  explicit CountMinBasicInt(const size_t bucket_size)
      : seed_(streamingcc::util::rand_int()), buffer_(bucket_size, 0) {}
  void ProcessItem(const uint32_t item, const double weight) override;
  double GetEstimation(const uint32_t item) const;
  ~CountMinBasicInt() {}

 private:
  uint32_t seed_;
  std::vector<uint32_t> buffer_;
};



// Count-Min sketch for weighted integer data stream.
class CountMinInt: public StreamingAlgorithmWeightedInt {
 public:
  CountMinInt(): CountMinInt(100, 20) {}
  explicit CountMinInt(const size_t bucket_size, const size_t num_copies);
  void ProcessItem(const uint32_t item, const double weight) override;
  // Estimate the total weights (frequency) of the given item
  double GetEstimation(const uint32_t item) const;
  
  ~CountMinInt() {}

 private:
  std::vector<CountMinBasicInt> cm_basic_;
};

}  // namespace integer



template <typename T>
class CountMin: public StreamingAlgorithmWeighted<T> {
 public:
  explicit CountMin(const size_t bucket_size, const size_t num_copies)
      : cm_int_(bucket_size, num_copies) {}
  void ProcessItem(const T& item, const double weight) override {
    cm_int_.ProcessItem(hash_(item), weight);
  }

  double GetEstimation(const T& item) const {
    return cm_int_.GetEstimation(hash_(item));
  }

  ~CountMin() {}

 private:
  integer::CountMinInt cm_int_;
  std::hash<T> hash_;
};

}  // namespace streamingcc


#endif  // SRC_STREAMINGCC_INCLUDE_COUNT_MIN_H_



