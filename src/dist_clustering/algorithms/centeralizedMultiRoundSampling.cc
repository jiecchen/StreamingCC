
#include "centeralizedMultiRoundSampling.h"
#include "util.h"

#include <algorithm>  //std::min

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../experiments/erfanUtil.h"
//

using namespace arma;

void Kmeanspp::perform(const ClusteringInput& input, ClusteringOutput& output) {
  CenteralizedMultiRoundSamplingClustering cmrs(input.n_centers, squareDist, 1);
  cmrs.perform(input, output);
}

void CenteralizedMultiRoundSamplingClustering::perform(
    const ClusteringInput& input, ClusteringOutput& output) {
  std::vector<arma::vec> currentCenters;
  arma::vec weights;
  arma::vec rawWeights;

  if (input.weights.size() == input.getNrows()) {
    rawWeights = input.weights;
  } else {
    rawWeights = arma::vec(input.getNrows(), fill::ones);
  }
  weights = rawWeights;
  for (int round = 0; round < n_rounds; round++) {
    std::vector<arma::vec> rCenters;
    std::vector<int> rInds;
    util::randomList(rInds, weights, n_centerPerRound);
    for (auto ind : rInds) {
      rCenters.push_back(input.data[ind]);
    }
    assert(rCenters.size() == (uint)n_centerPerRound);
    currentCenters.insert(currentCenters.end(), rCenters.begin(),
                          rCenters.end());
    std::vector<int> res_idx;
    std::vector<double> min_dist;
    util::compute_nearest(input.data, currentCenters, res_idx, min_dist);
    for (int i = 0; i < input.getNrows(); i++) {
      weights[i] = rawWeights[i] * min_dist[i];
      if (squareDist) weights[i] *= min_dist[i];
    }
  }
  output.centers =
      currentCenters;  // TODO go put this line out of loop in distAlg
}
