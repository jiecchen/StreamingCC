// Implementation for the paper, K-Mean--: A unified approach to clustering and
// outlier detection

#ifndef __KMEAN_MM__
#define __KMEAN_MM__

#include "algo_util.h"

#include <exception>
#include <vector>

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

// util functions for KMeanMinusMinus
namespace KMMM {
// calculate the difference between C1 and C2
// C1.size() == C2.size()
double diff_centers(const std::vector<arma::vec>& C1,
                    const std::vector<arma::vec>& C2);

std::vector<arma::vec> compute_centers(const std::vector<arma::vec>& U,
                                       const std::vector<int>& labels,
                                       const std::vector<int>& O,
                                       const int n_centers);

template <typename T>
std::vector<int> expand_points(std::vector<T>& U, const arma::vec& weights) {
  std::vector<int> corr_indices;
  auto UU = U;
  if (U.size() != weights.size()) {
    throw std::runtime_error("ERROR: |weights| != |data|");
  }
  U.clear();
  for (int i = 0; i < weights.size(); ++i) {
    for (int j = 0; j < weights[i]; ++j) {
      U.push_back(UU[i]);
      corr_indices.push_back(i);
    }
  }
  return corr_indices;
}
};

class ErfanMM : public ClusteringAlgo {
 public:
  ErfanMM()
      : kmeansppSeeding(false), max_iters_(100), thresh_(0.1) {}
  ErfanMM(bool kmeansppSeeding)
      : max_iters_(100), thresh_(0.1) {
    this->kmeansppSeeding = kmeansppSeeding;
  }
  ErfanMM(int max_iters, double thresh, bool kmeansppSeeding) {
    max_iters_ = max_iters;
    thresh_ = thresh;
    verbose = false;
    this->kmeansppSeeding = kmeansppSeeding;
  }
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  void setMaxIter(int max_iters) { max_iters_ = max_iters; }
  void setThresh(int thresh) { thresh_ = thresh; }
  std::string paramStr() override {
    std::stringstream ostream;
    ostream << max_iters_ << " " << thresh_ << " " << kmeansppSeeding;
    return ostream.str();
  }
  bool kmeansppSeeding;

 private:
  int max_iters_;
  double thresh_;
};

class WeightedKMeanMM : public ClusteringAlgo {
 public:
  WeightedKMeanMM() : max_iters_(100), thresh_(0.1) {}
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  void setMaxIter(int max_iters) { max_iters_ = max_iters; }
  void setThresh(int thresh) { thresh_ = thresh; }
  std::string paramStr() override {
    std::stringstream ostream;
    ostream << max_iters_ << " " << thresh_;
    return ostream.str();
  }

 private:
  int max_iters_;
  double thresh_;
};

// One of the issues for K-Mean-- is that once
// an outlier is sampled as a center, it always
// stays as the center.
// TODO: How to solve this issue elegantly?
class KMeanMinusMinus : public ClusteringAlgo {
 public:
  KMeanMinusMinus() : max_iters_(100), thresh_(0.1) {}
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  void setMaxIter(int max_iters) { max_iters_ = max_iters; }
  void setThresh(int thresh) { thresh_ = thresh; }
  std::string paramStr() override {
    std::stringstream ostream;
    ostream << max_iters_ << " " << thresh_;
    return ostream.str();
  }

 private:
  int max_iters_;
  double thresh_;
};





class FastWeightedKMeanMM : public ClusteringAlgo {
 public:
  FastWeightedKMeanMM()
    : kmeansppSeeding(true), max_iters_(100), thresh_(0.1) {}
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  void setMaxIter(int max_iters) { max_iters_ = max_iters; }
  void setThresh(int thresh) { thresh_ = thresh; }
  void setKmeanppSeeding(bool seeding) {
    kmeansppSeeding = seeding;
  }

  std::string paramStr() override {
    std::stringstream ostream;
    ostream << max_iters_ << " " << thresh_ << " " << kmeansppSeeding;
    return ostream.str();
  }
  bool kmeansppSeeding;

 private:
  int max_iters_;
  double thresh_;
};

#endif
