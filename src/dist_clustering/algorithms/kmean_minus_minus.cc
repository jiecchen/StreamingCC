#include "kmean_minus_minus.h"
#include "kmean_plus_plus.h"
#include "../experiments/erfanUtil.h"
#include "centeralizedMultiRoundSampling.h"
#include "util.h"

#include <cassert>
#include <exception>
#include <set>
#include <unordered_map>
#include <vector>

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

using namespace std;

vector<arma::vec> KMMM::compute_centers(const vector<arma::vec>& U,
                                        const vector<int>& labels,
                                        const std::vector<int>& O,
                                        const int n_centers) {
  set<int> outliers(O.begin(), O.end());
  int dim = U[0].size();
  arma::vec zero = arma::zeros<arma::vec>(dim);
  vector<arma::vec> centers(n_centers);
  vector<int> ct(n_centers);
  fill(centers.begin(), centers.end(), zero);

  for (int i = 0; i < U.size(); ++i) {
    if (outliers.count(i) > 0) {
      continue;
    }
    centers[labels[i]] += U[i];
    ct[labels[i]]++;
  }
  for (int i = 0; i < n_centers; ++i) {
    if (ct[i] > 0) {
      centers[i] /= ct[i];
    }
  }
  return centers;
}

vector<arma::vec> compute_centers(const vector<arma::vec>& U,
                                  const arma::vec& weights,
                                  const vector<int>& labels,
                                  const vector<int>& O,
                                  const vector<double>& Ow,
                                  const int n_centers) {
  unordered_map<int, double> outliers;
  for (int i = 0; i < O.size(); ++i) {
    outliers[O[i]] = Ow[i];
  }
  int dim = U[0].size();
  arma::vec zero = arma::zeros<arma::vec>(dim);
  vector<arma::vec> centers(n_centers);
  vector<double> ct(n_centers, 0);
  fill(centers.begin(), centers.end(), zero);

  
  
  for (int i = 0; i < U.size(); ++i) {
    double w = weights[i];
    if (outliers.find(i) != outliers.end()) {
      if (outliers[i] >= weights[i]) {
        continue;
      } else {
        w = weights[i] - outliers[i];
      }
    }

    centers[labels[i]] += w * U[i];
    ct[labels[i]] += w;
  }
  
  for (int i = 0; i < n_centers; ++i) {
    if (ct[i] > 0.0001) {
      centers[i] /= ct[i];
    }
  }
  return centers;
}

double KMMM::diff_centers(const vector<arma::vec>& C1,
                          const vector<arma::vec>& C2) {
  double c = 0;
  if (C1.size() != C2.size() || C1[0].size() != C2[0].size()) {
    return -1;  // error
  }

  //  cerr << "C1 & C2" << endl;
  for (int i = 0; i < C1.size(); ++i) {
    //    cerr << C1[i] << ", " << C2[i] << endl;
    c += util::dist(C1[i], C2[i]);
  }
  return c;
}

// return k centers
vector<arma::vec> smart_kmeanpp_seeding(const vector<arma::vec>& U,
					const vector<double>& weights,
					const int k) {
  ClusteringInput input;
  ClusteringOutput output;
  vector<double> ws;
  if (U.size() != weights.size()) {
    throw runtime_error("smart_kmeanpp_seeding: size error");
  }
  for (size_t i = 0; i < U.size(); ++i) {
    if (weights[i] > 1) {
      input.data.push_back(U[i]);
      ws.push_back(weights[i]);
    }
  }
  if (util::sum(ws) < k + 1) {
    throw runtime_error("smart_kmeanpp_seeding: weigths error");
  }
  input.n_centers = k;
  input.n_outliers = 0;
  input.weights = arma::vec(ws);
  KMeanPP algo;
  algo.perform(input, output);
  if (output.centers.size() != k) {
    throw runtime_error("smart_kmeanpp_seeding: output size error");
  }
  return output.centers;
}


void FastWeightedKMeanMM::perform(const ClusteringInput& input,
                                  ClusteringOutput& output) {
  // TODO: add more check
  double total_weights = arma::sum(input.weights);
  if (total_weights <= input.n_centers) {
    cerr << "total_weights = " << total_weights << endl;
    cerr << "n_centers = " << input.n_centers << endl;
    throw runtime_error("FastWeightedKMeanMM: |data| <= n_centers");
  }

  if (input.weights.size() != input.data.size()) {
    throw runtime_error("FastWeightedKMeanMM: invalid weights");
  }

  const auto& U = input.data;

  // TODO: sample
  auto&& weights_vec = util::arma_vec_to_vector(input.weights);
  auto&& C =
    util::subset(U, util::weighted_sample_without_repl(weights_vec, input.n_centers));
  if (kmeansppSeeding) {
    //    Kmeanspp seedingAlg;
    //    KMeanPP seedingAlg;
    //    seedingAlg.perform(input, output);
    C = smart_kmeanpp_seeding(input.data, weights_vec, input.n_centers);
    //    output.centers.clear();
  }

  vector<arma::vec> old_centers;
  util::convert_from_mat(old_centers,
                         arma::zeros<arma::mat>(C.size(), U[0].size()));

  vector<int> O;         // the set of outliers
  vector<double> Ow;     // weights of outliers, sum(O_weights) == n_outliers
  vector<double> dists;  // dist(U, C)
  vector<int> labels;    // label for each points in U

  int n_iters = 0;
  double diff = 0;
  if(verbose)
    cerr << "INFO: Performing interation ..." << endl;
  do {
    if(verbose)cerr << "INFO: Round " << ++n_iters << " ------" << endl;
    if (n_iters > this->max_iters_) {
      if(verbose)cerr << "INFO: exceeds max_iters, break while loop" << endl;
      break;
    }


    
    diff = KMMM::diff_centers(C, old_centers);
    if(verbose)cerr << "INFO: diff = " << diff << endl;


    util::compute_nearest(U, C, labels, dists);
    // sum(Ow) == n_outliers
    O = util::weighted_outliers(dists, input.weights, input.n_outliers, Ow);
    old_centers = C;
    C = compute_centers(U, input.weights, labels, O, Ow, input.n_centers);
  } while (diff > this->thresh_);

  
  output.centers = C;
  output.outliers_indices = O;
}

void KMeanMinusMinus::perform(const ClusteringInput& input,
                              ClusteringOutput& output) {
  // check the dataset
  // TODO: add more check
  assert(input.data.size() > input.n_centers);

  // make a copy of the data
  auto U = input.data;

  // TODO: write a function that sample data points directly,
  // instead of returning a vector of indices
  auto idx_C = util::sample_without_repl(input.data.size(), input.n_centers);
  auto C = util::subset(U, idx_C);

  vector<arma::vec> old_centers;
  util::convert_from_mat(old_centers,
                         arma::zeros<arma::mat>(C.size(), U[0].size()));

  vector<int> O;  // set of outliers
  vector<double> min_dist;
  vector<int> labels;

  int n_iter = 0;
  double diff = 0;
  if(verbose)cerr << "INFO: Performing interation ..." << endl;
  do {
    if(verbose)cerr << "INFO: Round " << ++n_iter << " ------" << endl;
    if (n_iter > this->max_iters_) {
      if(verbose)cerr << "INFO: exceeds max_iters, break while loop" << endl;
      break;
    }
    diff = KMMM::diff_centers(C, old_centers);
    if(verbose)cerr << "INFO: diff = " << diff << endl;

    util::compute_nearest(U, C, labels, min_dist);
    O = util::indices_of_upper(min_dist, input.n_outliers);
    old_centers = C;
    C = KMMM::compute_centers(U, labels, O, input.n_centers);
  } while (diff > this->thresh_);

  output.outliers_indices = O;
  util::compute_nearest(U, C, labels, min_dist);
  output.centers = C;

  // compute weights for the centers
  util::remove_subset(labels, O);
  unordered_map<int, int> weights;
  for (int i : labels) {
    weights[i]++;
  }
  assert(weights.size() == input.n_centers);
  vector<double> vec_weights;
  for (int i = 0; i < input.n_centers; ++i) {
    vec_weights.push_back(weights[i]);
  }
  output.weights = arma::vec(vec_weights);
}

void WeightedKMeanMM::perform(const ClusteringInput& input,
                              ClusteringOutput& output) {
  if (input.weights.size() != input.data.size()) {
    throw runtime_error("ERROR: |weights| != |data|");
  }

  KMeanMinusMinus kmmm;
  kmmm.setMaxIter(max_iters_);
  kmmm.setThresh(thresh_);
  auto input_copy = input;
  auto&& corr_indices = KMMM::expand_points(input_copy.data, input.weights);
  kmmm.perform(input_copy, output);
  auto& o_idx = output.outliers_indices;
  for (int& i : o_idx) {
    i = corr_indices[i];
  }
  // remove duplicates
  set<int> outliers_set(o_idx.begin(), o_idx.end());
  vector<int> tmp(outliers_set.begin(), outliers_set.end());
  o_idx = tmp;
  
  // the outlier_indices is no longer important
  // output.outliers_indices.clear();
}

void ErfanMM::perform(const ClusteringInput& input, ClusteringOutput& output) {
  if (input.weights.size() != input.data.size()) {
    throw runtime_error("ERROR: |weights| != |data|");
  }
  std::vector<int> centerIndices;
  std::vector<arma::vec> currentCenters;
  if (kmeansppSeeding) {
    Kmeanspp seedingAlg;
    seedingAlg.perform(input, output);
    currentCenters = output.centers;
  } else {
    int sss = input.weights.n_elem;
    util::randomList(centerIndices, input.weights, input.n_centers);
    currentCenters = util::subset(input.data, centerIndices);
  }
  arma::vec currentWeights(input.getNrows());

  vector<arma::vec> oldCenters;
  vector<double> min_dist;
  int n_iter = 0;
  double diff = 0;
  if (verbose) cerr << "INFO: Performing interation ..." << endl;
  do {
    if(verbose)cerr << "INFO: Round " << ++n_iter << " ------" << endl;
    if (n_iter > this->max_iters_) {
      if(verbose)cerr << "INFO: exceeds max_iters, break while loop" << endl;
      break;
    }
    //    printLine;
    //    VARd(diff);
    if (oldCenters.size() == 0)
      diff = 1000;
    else
      diff = KMMM::diff_centers(currentCenters, oldCenters);
    if(verbose)cerr << "INFO: diff = " << diff << endl;

    util::compute_nearest(input.data, currentCenters, output.labels, min_dist);
    util::weights_of_upper(currentWeights, min_dist, input.weights,
                           (double)(input.n_outliers));

    oldCenters = currentCenters;
    util::average_centers(currentCenters, input.data, currentWeights,
                          output.labels, output.outliers_indices,
                          input.n_centers, oldCenters);

  } while (diff > this->thresh_);

  output.centers = currentCenters;
}
