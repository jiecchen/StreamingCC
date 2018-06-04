// kmeanspp
#ifndef __MUTLIROUNDSAMPLING_H__
#define __MUTLIROUNDSAMPLING_H__

#include "../algorithms/algo_util.h"

class MultiRoundSamplingClustering : public ClusteringAlgo {
 public:
  MultiRoundSamplingClustering() {}
  MultiRoundSamplingClustering(const int n_rounds, const bool squareDist,
                               const int n_nodes, const int node_id,
                               const int root, int n_local_centers) {
    this->n_rounds = n_rounds;
    this->squareDist = squareDist;
    this->n_nodes = n_nodes;
    this->node_id = node_id;
    this->root = root;
    this->n_local_centers = n_local_centers;
  }
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  std::string paramStr() {
    std::stringstream ostream;
    ostream << n_rounds << " " << squareDist << " " << n_local_centers;
    return ostream.str();
  }
  //  int n_nodes; exists for all algs
  int node_id;
  int root;
  int n_local_centers;
  int n_rounds;
  bool squareDist;
};

class TwoPhaseClustering : public ClusteringAlgo {
 public:
  TwoPhaseClustering() {}
  TwoPhaseClustering(ClusteringAlgo* firstAlgo,
                     ClusteringAlgo* reclusteringAlgo, const int n_nodes,
                     const int node_id, const int root) {
    this->firstAlgo = firstAlgo;
    this->reclusteringAlgo = reclusteringAlgo;
    this->n_nodes = n_nodes;
    this->node_id = node_id;
    this->root = root;
    //    this->n_local_centers=n_local_centers;
  }
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  int node_id;
  int root;
  ClusteringAlgo* firstAlgo;
  ClusteringAlgo* reclusteringAlgo;
  std::string paramStr() {
    std::stringstream ostream;
    ostream << firstAlgo->getName() << " " << firstAlgo->paramStr() << " "
            << reclusteringAlgo->getName() << " "
            << reclusteringAlgo->paramStr();
    return ostream.str();
  }
};

#endif
