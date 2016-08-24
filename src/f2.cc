// Copyright 2016 Jiecao Chen

#include "streamingcc_include/f2.h"
#include "streamingcc_include/hash.h"
#include "streamingcc_include/util.h"

#include <vector>
#include <ctime>
#include <cstdlib>

namespace streamingcc {
namespace integer {

F2Int::F2Int(const size_t bucket_size, const size_t num_copies) {
  for (size_t i = 0; i < num_copies; ++i) {
    f2_basic.push_back(F2BasicInt(bucket_size));
  }
}

void F2Int::ProcessItem(const ItemType item, const double weight) {
  // TODO(jiecchen): make it run in parallel
  for (auto& f2basic : f2_basic) {
    f2basic.ProcessItem(item, weight);
  }
}

double F2Int::GetEstimation() const {
  std::vector<double> estimations(f2_basic.size(), 0);
  
  for (size_t i = 0; i < f2_basic.size(); ++i) {
    estimations[i] = f2_basic[i].GetEstimation();
  }
  return util::CalcMedian(estimations);
}

F2BasicInt::F2BasicInt(const size_t bucket_size) {
  buckets.reserve(bucket_size);
  std::srand(std::time(0));
  for (size_t i = 0; i < bucket_size; ++i) {
    random_seeds.push_back(std::rand());
  }
}

void F2BasicInt::ProcessItem(const ItemType item, const double weight) {
  for (size_t i = 0 ; i < random_seeds.size(); ++i) {
    int sign = (murmurhash(item, random_seeds[i]) % 2) * 2 - 1;
    buckets[i] += sign * weight;
  }
}

double F2BasicInt::GetEstimation() const {
  double tot = 0;
  for (const auto x : buckets) {
    tot += x * x;
  }
  return tot / buckets.size();
}

}  // namespace integer
}  // namespace streamingcc










