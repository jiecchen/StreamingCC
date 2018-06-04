// The implementation of scalable kmeans++ aka kmeans||
// d

#include "multiRoundSampling.h"
#include "../algorithms/util.h"
#include "distAlgo_util.h"

#include <mpi.h>
#include <algorithm>  //std::min

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../experiments/erfanUtil.h"
//

using namespace arma;
/*
void print_vectord(const std::vector<double>& vector){
  for (auto i = vector.begin(); i != vector.end(); ++i)
    std::cout << *i << ' ';
  std::cout<<std::endl;
}
*/

void MultiRoundSamplingClustering::perform(const ClusteringInput &input,
                                           ClusteringOutput &output) {
  int length = input.getNrows() / n_nodes;
  int firstInd = node_id * length;
  int lastInd = std::min((node_id + 1) * length, input.getNrows());
  auto firstItr = input.data.begin() + firstInd;
  auto lastItr = input.data.begin() + lastInd;
  ClusteringInput myInput(std::vector<arma::vec>(firstItr, lastItr),
                          input.n_centers, 0);
  if (input.weights.size() ==
      (uint)input.getNrows()) {  // if the orginal input was weighted use them
    myInput.weights = input.weights.subvec(firstInd, lastInd - 1);
  }
  std::vector<arma::vec> currentCenters;
  arma::vec weights = arma::vec(myInput.getNrows(), fill::ones);

  for (int round = 0; round < n_rounds; round++) {
    std::vector<arma::vec> rCenters;
    std::vector<int> rInds;
    util::randomList(rInds, weights, n_local_centers);
    for (auto ind : rInds) {
      rCenters.push_back(myInput.data[ind]);
    }
    assert(rCenters.size() == (uint)n_local_centers);
    int msgSize = rCenters.size() * myInput.getNcols();
    double *sendbuf = (double *)malloc(msgSize * sizeof(double));
    util::copy_to_array(sendbuf, rCenters);
    double *recvbuf = (double *)malloc(n_nodes * msgSize * sizeof(double));
    allGatherCenters(recvbuf, sendbuf, msgSize, node_id, n_nodes, root);
    int nrc = n_nodes * n_local_centers;  // number of received centers
    for (int i = 0; i < nrc; i++) {
      currentCenters.push_back(
          arma::vec(recvbuf + i * myInput.getNcols(), myInput.getNcols()));
    }
    free(sendbuf);
    free(recvbuf);

    std::vector<int> res_idx;
    std::vector<double> min_dist;
    util::compute_nearest(myInput.data, currentCenters, res_idx, min_dist);
    for (int i = 0; i < myInput.getNrows(); i++) {
      weights[i] = myInput.weights[i] * min_dist[i];
      if (squareDist) weights[i] *= min_dist[i];
    }
  }
  output.centers = currentCenters;
  output.fillExtraInfo(myInput);
}

void TwoPhaseClustering::perform(const ClusteringInput &input,
                                 ClusteringOutput &output) {
  //  assert(input.n_outliers==0);
  ClusteringOutput intermediateOutput;

  firstAlgo->perform(input, intermediateOutput);

  int msgSize = intermediateOutput.centers.size() * (input.getNcols() + 2);
  double *sendbuf = (double *)malloc(msgSize * sizeof(double));
  int sendSize = 0;
  prepareSendArray(sendbuf, sendSize, intermediateOutput, input);
  double *recvbuf = NULL;
  if (node_id == root)
    recvbuf = (double *)malloc(n_nodes * msgSize * sizeof(double));
  gatherArrsToRoot(recvbuf, msgSize, sendbuf, msgSize, node_id, n_nodes, root);
  free(sendbuf);
  if (node_id == root) {
    ClusteringInput intermediateInput;
    unpackRecvArray(recvbuf, msgSize, intermediateInput, n_nodes);
    //    intermediateOutput.fillExtraInfo(input);
    intermediateInput.n_centers = input.n_centers;
    intermediateInput.n_outliers = input.n_outliers;
    assert(intermediateInput.weights.size() ==
           ((uint)intermediateInput.getNrows()));
    free(recvbuf);
    reclusteringAlgo->perform(intermediateInput, output);
    output.num_intermediate_centers = intermediateInput.data.size();
  }
}
