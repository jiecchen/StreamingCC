#ifndef __PARTITIONING_H__
#define __PARTITIONING_H__

#include "../algorithms/algo_util.h"
#include "../algorithms/util.h"
#define ARMA_DONT_USE_WRAPPER
#include <algorithm>
#include "../arma/include/armadillo"
#include "../experiments/erfanUtil.h"
#include "distAlgo_util.h"

class PartitionClustering : public ClusteringAlgo {
  // partitions the input and runs local clustering algo and then global
 public:
  PartitionClustering() {}
  PartitionClustering(ClusteringAlgo* localClusteringAlgo,
                      ClusteringAlgo* globalClusteringAlgo, const int n_nodes,
                      const int node_id, const int root, int n_local_centers, bool onlyCenters=false) {
    this->localClusteringAlgo = localClusteringAlgo;
    this->globalClusteringAlgo = globalClusteringAlgo;
    this->n_nodes = n_nodes;
    this->node_id = node_id;
    this->root = root;
    this->n_local_centers = n_local_centers;
    this->onlyCenters=onlyCenters;
  }

  ClusteringAlgo* localClusteringAlgo;
  ClusteringAlgo* globalClusteringAlgo;  // should support weighted input
                                         //  int n_nodes; exists for all algs
  int node_id;
  int root;
  int n_local_centers;
  bool onlyCenters;
  void perform(const ClusteringInput& input, ClusteringOutput& output) override;
  std::string paramStr() {
    std::stringstream ostream;
    ostream << localClusteringAlgo->getName() << " "
            << localClusteringAlgo->paramStr() << " "
            << globalClusteringAlgo->getName() << " "
            << globalClusteringAlgo->paramStr();
    return ostream.str();
  }
};

#endif
