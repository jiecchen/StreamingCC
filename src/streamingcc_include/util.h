// Copyright 2016 Jiecao Chen
#ifndef SRC_STREAMINGCC_INCLUDE_UTIL_H_
#define SRC_STREAMINGCC_INCLUDE_UTIL_H_

#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>
#include <random>

namespace streamingcc {
namespace util {

// randomly return from 0, 1, 2, ..., M-1
uint32_t rand_int(int M = 1 << 30);

template <class T>
T CalcMedian(const std::vector<T>& vec_original) {
  std::vector<T> vec(vec_original.begin(), vec_original.end());
  assert(!vec.empty());
  std::nth_element(vec.begin(), vec.begin() + vec.size() / 2, vec.end());
  return vec[vec.size() / 2];
}

}  // namespace util
}  // namespace streamingcc

#endif  // SRC_STREAMINGCC_INCLUDE_UTIL_H_










