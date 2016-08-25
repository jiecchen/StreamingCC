// Copyright 2016 Jiecao Chen
#include "streamingcc_include/count_min.h"

#include <vector>
#include <cstdint>
#include <cassert>
#include <algorithm>

#include "streamingcc_include/hash.h"
#include "streamingcc_include/util.h"


namespace streamingcc {
namespace integer {

using streamingcc::util::rand_int;

CountMinInt::CountMinInt(const size_t bucket_size, const size_t num_copies) {
  assert(bucket_size > 0);
  assert(num_copies > 0);
  for (size_t i = 0; i < num_copies; ++i) {
    random_seeds_.push_back(rand_int());
    buffers_.push_back(std::vector<uint32_t>(bucket_size, 0));
  }
}

void CountMinInt::ProcessItem(const uint32_t item, const double weight) {
  assert(weight >= 0);
  const auto kBucketSize = buffers_[0].size();
  size_t i = 0;
  for (const auto seed : random_seeds_) {
    auto p = murmurhash(item, seed) % kBucketSize;
    buffers_[i][p] += weight;
    ++i;
  }
}

double CountMinInt::GetEstimation(const uint32_t item) const {
  std::vector<double> values(random_seeds_.size(), 0);
  const auto kBucketSize = buffers_[0].size();
  size_t i = 0;
  for (const auto seed : random_seeds_) {
    auto p = murmurhash(item, seed) % kBucketSize;
    values[i] = buffers_[i][p];
    ++i;
  }
  return *std::min_element(values.begin(), values.end());
}

}  // namespace integer
}  // namespace streamingcc




