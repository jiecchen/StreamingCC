// Copyright 2016 Jiecao Chen
#ifndef SRC_STREAMINGCC_INCLUDE_COUNT_MIN_H_
#define SRC_STREAMINGCC_INCLUDE_COUNT_MIN_H_

#include <vector>
#include <cstdint>
#include <cstdlib>
#include <memory>

#include "../streamingcc_include/streaming_algorithm.h"

namespace streamingcc {
namespace integer {

// Count-Min sketch for weighted integer data stream.
class CountMinInt: public StreamingAlgorithmWeightedInt {
 public:
  CountMinInt(): CountMinInt(100, 20) {}
  explicit CountMinInt(const size_t bucket_size, const size_t num_copies);
  void ProcessItem(const uint32_t item, const double weight) override;
  // Estimate the total weights (frequency) of the given item
  double GetEstimation(const uint32_t item) const;

 private:
  std::vector<uint32_t> random_seeds_;
  std::vector<std::vector<uint32_t>> buffers_;
};

}  // namespace integer
}  // namespace streamingcc


#endif  // SRC_STREAMINGCC_INCLUDE_COUNT_MIN_H_



