// Copyright 2016 Jiecao Chen

#ifndef SRC_STREAMINGCC_INCLUDE_F2_H_
#define SRC_STREAMINGCC_INCLUDE_F2_H_

#include <vector>
#include <cstdlib>
#include <memory>

#ifdef __clang__
  #include <tr1/cstdint>
#else
  #include <cstdint>
#endif

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
  std::vector<uint32_t> random_seeds_;
  std::vector<double> buckets_;
};

// A aggregated version of F2 sketch for integer data stream. The
// variance of the estimation is reduced by taking the median of
// num_copies of F2BasicInt.
class F2Int: public StreamingAlgorithmWeightedInt {
 public:
  F2Int(): F2Int(100, 10) {}
  explicit F2Int(const size_t bucket_size, const size_t num_copies = 10);

  // Processes an item
  void ProcessItem(const uint32_t item, const double weight = 1.) override;

  // Gets the estimation of self-join size
  double GetEstimation() const;
  ~F2Int() override {}

 private:
  std::vector<F2BasicInt> f2_basic_;
};



}  // namespace integer

template <typename T>
class F2: public StreamingAlgorithmWeighted<T> {
 public:
  explicit F2(const size_t bucket_size, const size_t num_copies)
      : f2_int_(bucket_size, num_copies) {}

  void ProcessItem(const T& item, const double weight) override {
    f2_int_.ProcessItem(hash_(item), weight);
  }

  double GetEstimation() const {
    return f2_int_.GetEstimation();
  }
  
 private:
  std::hash<T> hash_;
  integer::F2Int f2_int_;
};

}  // namespace streamingcc

#endif  // SRC_STREAMINGCC_INCLUDE_F2_H_













