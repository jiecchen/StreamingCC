// Copyright 2016 Jiecao Chen
#ifndef SRC_STREAMINGCC_INCLUDE_SAMPLING_H_
#define SRC_STREAMINGCC_INCLUDE_SAMPLING_H_

#include <vector>
#include "../streamingcc_include/streaming_algorithm.h"
#include "../streamingcc_include/util.h"


namespace streamingcc {


template <typename T>
class ReservoirSampler: public StreamingAlgorithm<T> {
 public:
  ReservoirSampler(const int n_samples,
                   bool is_with_replacement = true):
      counter_(0), with_rpmt_(is_with_replacement),
      n_samples_(n_samples) {}

  void ProcessItem(const T& item) override {
    counter_++;
    if (with_rpmt_) {
      ProcessItemWithRpmt(item);
    } else {
      ProcessItemWithoutRpmt(item);
    }
  }

  std::vector<T> GetSamples() const {
    return samples_;
  }

  ~ReservoirSampler() override {}

 private:
  void ProcessItemWithRpmt(const T& item) {
    if (samples_.empty()) {
      for (int i = 0; i < n_samples_; ++i) {
        samples_.push_back(item);
      }
    } else {
      for (auto& sp : samples_) {
        if (util::rand_double(1.) < 1. / counter_) {
          sp = item;
        }
      }
    }
  }
  void ProcessItemWithoutRpmt(const T& item) {
    if (samples_.size() < n_samples_) {
      samples_.push_back(item);
    } else {
      if (util::rand_double(1.) < 1. / counter_) {
        int p = util::rand_int(counter_);
        samples_[p] = item;
      }
    }
  }
  int64_t counter_;
  const bool with_rpmt_;
  const int n_samples_;  // number of samples being kept
  std::vector<T> samples_;
};

}  // namespace streamingcc

#endif  // SRC_STREAMINGCC_INCLUDE_SAMPLING_H_
