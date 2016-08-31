// Copyright 2016 Jiecao Chen
#include "streamingcc_include/count_min.h"

#include <vector>
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
    cm_basic_.push_back(CountMinBasicInt(bucket_size));
  }
}

void CountMinInt::ProcessItem(const uint32_t item, const double weight) {
  assert(weight >= 0);
  for (auto& cm : cm_basic_) {
    cm.ProcessItem(item, weight);
  }
}

double CountMinInt::GetEstimation(const uint32_t item) const {
  std::vector<double> values;
  for (const auto& cm : cm_basic_) {
    values.push_back(cm.GetEstimation(item));
  }
  return *std::min_element(values.begin(), values.end());
}


void CountMinBasicInt::ProcessItem(const uint32_t item, const double weight) {
  assert(weight >= 0);
  auto p = murmurhash(item, seed_) % buffer_.size();
  buffer_[p] += weight;
}

double CountMinBasicInt::GetEstimation(const uint32_t item) const {
  auto p = murmurhash(item, seed_) % buffer_.size();
  return buffer_[p];
}



}  // namespace integer
}  // namespace streamingcc




