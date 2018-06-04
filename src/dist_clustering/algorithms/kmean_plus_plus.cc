#include "kmean_plus_plus.h"
#include "util.h"

#include <vector>
#include <random>
#include <ctime>
#include <exception>
#include <cstdlib>

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

using namespace std;



int weighted_sample(const vector<double>& weights, default_random_engine& gen) {
  double tot_ws = util::sum(weights);
  if (tot_ws <= 0) {
    throw runtime_error("weighted_sample: weights error");
  }
  discrete_distribution<int> sampler(weights.begin(), weights.end());
  return sampler(gen);
}

void KMeanPP::perform(const ClusteringInput& input,
		      ClusteringOutput& output) {
  const auto& U = input.data;
  auto& C = output.centers;
  auto& indices = output.centers_indices;
  
  C.clear();
  indices.clear();
  const int coreset_size = input.n_centers;
  
  auto&& tmp = util::sample_with_repl(U.size(), 1);
  indices.push_back(tmp[0]);
  C.push_back(U[tmp[0]]);
  
  vector<double> dists;
  vector<double> sq_dists;
  vector<int> labels;

  util::compute_nearest(U, C, labels, dists);
  for (double a : dists) {
    sq_dists.push_back(a * a);
  }

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator(seed);

  for (int j = 1; j < coreset_size; ++j) {
    //    auto&& tmp = util::weighted_sample_with_repl(sq_dists, 1);
    //    int idx = tmp[0];
    int idx = weighted_sample(sq_dists, generator);
    
    C.push_back(U[idx]);
    indices.push_back(idx);
    // update dists and sq_dists
    for (int i = 0; i < U.size(); ++i) {
      double d = util::dist(U[i], U[idx]);
      if (d < dists[i]) {
	dists[i] = d;
	sq_dists[i] = d * d;
      }
    }
  }
  
  // compute weights
  auto&& ws = util::compute_weights(U, C);
  output.weights = arma::vec(ws);
}
