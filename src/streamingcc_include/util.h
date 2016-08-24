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
  
constexpr int MAX = 1 << 30;
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<> dis(0, 1);
static std::default_random_engine generator;
static std::uniform_int_distribution<> dis_int(0, MAX);


// randomly return from 0, 1, 2, ..., M-1
uint32_t rand_int(int M = 1 << 30);


void test();

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










