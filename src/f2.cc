// Copyright 2016 Jiecao Chen
#include "streamingcc_include/f2.h"

#include <vector>
#include <cstdlib>

#include "streamingcc_include/hash.h"
#include "streamingcc_include/util.h"

namespace streamingcc {
namespace integer {
using streamingcc::util::rand_int;

F2Int::F2Int(const size_t bucket_size, const size_t num_copies) {
  for (size_t i = 0; i < num_copies; ++i) {
    f2_basic_.push_back(F2BasicInt(bucket_size));
  }
}

void F2Int::ProcessItem(const uint32_t item, const double weight) {
  // TODO(jiecchen): make it run in parallel
  for (auto& f2basic : f2_basic_) {
    f2basic.ProcessItem(item, weight);
  }
}

double F2Int::GetEstimation() const {
  std::vector<double> estimations(f2_basic_.size(), 0);
  for (size_t i = 0; i < f2_basic_.size(); ++i) {
    estimations[i] = f2_basic_[i].GetEstimation();
  }
  return util::CalcMedian(estimations);
}

F2BasicInt::F2BasicInt(const size_t bucket_size)
    : buckets_(bucket_size, 0) {
  for (size_t i = 0; i < bucket_size; ++i) {
    random_seeds_.push_back(rand_int());
  }
}

void F2BasicInt::ProcessItem(const uint32_t item, const double weight) {
  for (size_t i = 0 ; i < random_seeds_.size(); ++i) {
    int sign = (murmurhash(item, random_seeds_[i]) % 2) * 2 - 1;
    buckets_[i] += sign * weight;
  }
}

double F2BasicInt::GetEstimation() const {
  double tot = 0;
  for (const auto x : buckets_) {
    tot += x * x;
  }
  return tot / buckets_.size();
}

}  // namespace integer
}  // namespace streamingcc










