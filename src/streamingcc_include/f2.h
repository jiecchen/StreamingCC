// Copyright 2016 Jiecao Chen

#ifndef SRC_STREAMINGCC_INCLUDE_F2_H_
#define SRC_STREAMINGCC_INCLUDE_F2_H_

#include <vector>
#include <cstdint>
#include <cstdlib>
#include <memory>

#include "../streamingcc_include/streaming_algorithm.h"

namespace streamingcc {
namespace integer {


// A basic version of F2 sketch for integer data stream.
class F2BasicInt: public StreamingAlgorithmWeightedInt {
 public:
  explicit F2BasicInt(const size_t bucket_size);
  // Processes an item
  void ProcessItem(const uint32_t item, const double weight) override;
  // Gets the estimation of self-join size
  double GetEstimation() const;
  ~F2BasicInt() override {}

 private:
  std::vector<uint32_t> random_seeds;
  std::vector<double> buckets;
};

// A aggregated version of F2 sketch for integer data stream. The
// variance of the estimation is reduced by taking the median of 
// num_copies of F2BasicInt.
class F2Int: public StreamingAlgorithmWeightedInt {
 public:
  explicit F2Int(const size_t bucket_size, const size_t num_copies = 10);

  // Processes an item
  void ProcessItem(const uint32_t item, const double weight) override;

  // Gets the estimation of self-join size
  double GetEstimation() const;
  ~F2Int() override {}

 private:
  std::vector<F2BasicInt> f2_basic;
};



}  // namespace integer
}  // namespace streamingcc

#endif  // SRC_STREAMINGCC_INCLUDE_F2_H_













