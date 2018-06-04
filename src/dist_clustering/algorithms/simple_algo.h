#ifndef _SIMPLE_ALGORITHMS_H
#define _SIMPLE_ALGORITHMS_H

#include "algo_util.h"

class RandomClustering : public ClusteringAlgo {
 public:
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
};

// Jiecao's implementation
class RandClustering : public ClusteringAlgo {
public:
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
};

class IdentityClustering : public ClusteringAlgo {
public:
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  int maxMsgSize(const ClusteringInput& input) override {
    return (input.getNrows()) * (input.getNcols() + 1) + 2;
  }
};

class ArmaKmeansClustering : public ClusteringAlgo {
 public:
  ArmaKmeansClustering() {}
  ArmaKmeansClustering(const int n_iter, bool verbose) {
    this->n_iter = n_iter;
    this->verbose = verbose;
  }
  int n_iter;
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;

  std::string paramStr() override {
    std::stringstream ostream;
    ostream << n_iter;
    return ostream.str();
  }
};

#endif
