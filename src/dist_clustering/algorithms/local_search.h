// Implementation for "Local Search Methods for k-Means with Outliers"
#ifndef __LOCAL_SEARCH_H__
#define __LOCAL_SEARCH_H__

#include "algo_util.h"

#include <vector>

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

std::vector<int> outliers(const std::vector<arma::vec>& U,
                          const std::vector<arma::vec>& centers,
                          const std::vector<int>& X, const int n_outliers);

double sum2(const std::vector<double>& nums, const arma::vec& weights);

double cost2(const std::vector<arma::vec>& U, const std::vector<arma::vec>& C,
             const int n_outliers = 0);
double cost2(const std::vector<arma::vec>& U, const std::vector<arma::vec>& C,
             const std::vector<int>& outliers);

double cost1(const std::vector<arma::vec>& U, const std::vector<arma::vec>& C,
             const int n_outliers = 0);
double cost1(const std::vector<arma::vec>& U, const std::vector<arma::vec>& C,
             const std::vector<int>& outliers);




std::vector<int> outliers(const std::vector<arma::vec>& U,
                          const std::vector<arma::vec>& C,
                          const arma::vec& weights, const int z,
                          std::vector<double>& Ow);

//
double cost_outliers(const std::vector<arma::vec>& U,
                     const std::vector<arma::vec>& centers,
                     const std::vector<int>& outliers);

// The local search algorithm for k-means with no outliers.
// One important property of this algorithm is that this algorithm
// never increases the cost.
void local_search(const std::vector<arma::vec>& dataset,
                  std::vector<arma::vec>& centers, const double eps = 0.0001);

void weighted_local_search(const std::vector<arma::vec>& dataset,
                           const arma::vec& weights,
                           std::vector<arma::vec>& centers,
                           const double eps = 0.0001);

class LocalSearchAlgo : public ClusteringAlgo {
 public:
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
};

class WeightedLocalSearchAlgo : public ClusteringAlgo {
 public:
  WeightedLocalSearchAlgo() :  max_iters_(3), eps_(0.1) {}
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  void setMaxIters(int max_iters) { max_iters_ = max_iters; }

  void setEps(double eps) { eps_ = eps; }

 private:
  int max_iters_;
  double eps_;
};

class FLSAlgo
    : public ClusteringAlgo {  // Fast Local Search: do kmeans++ first then
                               // weighted local search with outliers.
 public:
  FLSAlgo() : alpha(1), max_iters_(3) {}
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  void setMaxIters(int max_iters) { max_iters_ = max_iters; }
  std::string paramStr() override {
    std::stringstream ostream;
    ostream << max_iters_ << " " << alpha;
    return ostream.str();
  }
  int alpha;  // The number repeats of k+t

 private:
  int max_iters_;
};
#endif
