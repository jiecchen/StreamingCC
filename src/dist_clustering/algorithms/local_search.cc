#include "local_search.h"
#include "../experiments/erfanUtil.h"
#include "centeralizedMultiRoundSampling.h"
#include "util.h"

#include <cassert>
#include <exception>
#include <set>
#include <vector>

using namespace std;

double sum(const vector<double>& nums) {
  double sm = 0;
  for (auto a : nums) {
    sm += a;
  }
  return sm;
}

double sum2(const vector<double>& nums) {
  double sm = 0;
  for (auto a : nums) {
    sm += a * a;
  }
  return sm;
}

double sum2(const vector<double>& nums, const arma::vec& weights) {
  double sm = 0;
  if (nums.size() != weights.size()) {
    throw runtime_error("sum2: |nums| != |weights|");
  }
  for (int i = 0; i < nums.size(); ++i) {
    sm += weights[i] * nums[i] * nums[i];
  }
  return sm;
}

double sum2(const vector<double>& nums, int n_rm) {
  auto&& idx = util::indices_of_upper(nums, n_rm);
  return sum2(util::backslash(nums, idx));
}

vector<int> outliers(const vector<arma::vec>& U,
                     const vector<arma::vec>& centers, const vector<int>& X,
                     const int n_outliers) {
  int z = n_outliers;
  if (X.size() + n_outliers > U.size()) {
    cerr << "outliers: n_outliers is too large" << endl;
    z = U.size() - X.size();
  }
  vector<double> dists;
  vector<int> labels;
  util::compute_nearest(U, centers, labels, dists);
  for (int i : X) {
    dists[i] = -1;
  }
  return util::indices_of_upper(dists, z);
}

double cost_outliers(const vector<arma::vec>& U,
                     const vector<arma::vec>& centers,
                     const vector<int>& outliers) {
  vector<double> dists;
  vector<int> labels;
  util::compute_nearest(U, centers, labels, dists);
  util::remove_subset(dists, outliers);
  double c = 0;
  for (auto a : dists) {
    c += a * a;
  }
  return c;
}

// TODO: think of a way to optimize it.
// is it possible?

// try to swap centers[idx] and v if the cost decrease.
// update dists and labels when swap happens.
// return true if swap happens.
bool try_swap(const vector<arma::vec>& dataset, vector<arma::vec>& centers,
              const int idx, const arma::vec& v, vector<double>& dists,
              vector<int>& labels) {
  // backup
  auto copy_dists = dists;
  auto copy_labels = labels;
  auto copy_center = centers[idx];
  double old_cost = sum2(dists);

  // swap
  centers[idx] = v;

  for (int i = 0; i < dataset.size(); ++i) {
    if (labels[i] == idx) {
      double m = 1e30;
      for (int j = 0; j < centers.size(); ++j) {
        double c = util::dist(dataset[i], centers[j]);
        if (c < m) {
          labels[i] = j;
          dists[i] = c;
        }
      }  // for
    } else {
      double c = util::dist(dataset[i], centers[idx]);
      if (dists[i] > c) {
        dists[i] = c;
        labels[i] = idx;
      }
    }  // else
  }    // for

  double new_cost = sum2(dists);
  if (new_cost <= old_cost) {
    return true;
  }
  // no swap, need to recover
  dists = copy_dists;
  labels = copy_labels;
  centers[idx] = copy_center;
  return false;
}

bool try_swap(const vector<arma::vec>& dataset, const arma::vec& weights,
              vector<arma::vec>& centers, const int idx, const arma::vec& v,
              vector<double>& dists, vector<int>& labels) {
  // backup
  auto copy_dists = dists;
  auto copy_labels = labels;
  auto copy_center = centers[idx];
  double old_cost = sum2(dists, weights);

  // swap
  centers[idx] = v;

  for (int i = 0; i < dataset.size(); ++i) {
    if (labels[i] == idx) {
      double m = 1e30;
      for (int j = 0; j < centers.size(); ++j) {
        double c = util::dist(dataset[i], centers[j]);
        if (c < m) {
          labels[i] = j;
          dists[i] = c;
        }
      }  // for
    } else {
      double c = util::dist(dataset[i], centers[idx]);
      if (dists[i] > c) {
        dists[i] = c;
        labels[i] = idx;
      }
    }  // else
  }    // for

  double new_cost = sum2(dists, weights);
  if (new_cost < old_cost) {
    return true;
  }
  // no swap, need to recover
  dists = copy_dists;
  labels = copy_labels;
  centers[idx] = copy_center;
  return false;
}

double cost2(const vector<arma::vec>& U, const vector<arma::vec>& C,
             const int n_outliers) {
  vector<double> dists;
  vector<int> labels;
  util::compute_nearest(U, C, labels, dists);
  if (n_outliers > 0) {
    auto&& idx = util::indices_of_upper(dists, n_outliers);
    util::remove_subset(dists, idx);
  }
  double c = 0;
  for (auto a : dists) {
    c += a * a;
  }
  return c;
}


double cost2(const vector<arma::vec>& U, const vector<arma::vec>& C,
             const vector<int>& outliers) {
  vector<double> dists;
  vector<int> labels;
  util::compute_nearest(U, C, labels, dists);
  if (outliers.size() > 0) {
    util::remove_subset(dists, outliers);
  }
  double c = 0;
  for (auto a : dists) {
    c += a * a;
  }
  return c;
}


double cost1(const vector<arma::vec>& U, const vector<arma::vec>& C,
             const int n_outliers) {
  vector<double> dists;
  vector<int> labels;
  util::compute_nearest(U, C, labels, dists);
  if (n_outliers > 0) {
    auto&& idx = util::indices_of_upper(dists, n_outliers);
    util::remove_subset(dists, idx);
  }
  double c = 0;
  for (auto a : dists) {
    c += a;
  }
  return c;
}


double cost1(const vector<arma::vec>& U, const vector<arma::vec>& C,
             const vector<int>& outliers) {
  vector<double> dists;
  vector<int> labels;
  util::compute_nearest(U, C, labels, dists);
  if (outliers.size() > 0) {
    util::remove_subset(dists, outliers);
  }
  double c = 0;
  for (auto a : dists) {
    c += a;
  }
  return c;
}



// The local search algorithm for k-means with no outliers
void local_search(const vector<arma::vec>& dataset, vector<arma::vec>& centers,
                  const double eps) {
  // TODO: add some check
  const int k = centers.size();
  double alpha = 1e30;

  vector<double> dists;
  vector<int> labels;
  util::compute_nearest(dataset, centers, labels, dists);

  double cost = sum2(dists);
  //  double cost = cost2(dataset, centers);
  while (alpha * (1 - eps / k) > cost) {
    alpha = cost;
    auto C = centers;
    for (const auto& v : dataset) {
      for (int i = 0; i < k; ++i) {
        if (try_swap(dataset, C, i, v, dists, labels)) {
          cost = sum2(dists);
        }
        // const auto u = centers[i];
        // centers[i] = v;
        // double new_cost = cost2(dataset, centers);
        // if (new_cost < cost) {
        //   C = centers;
        //   cost = new_cost;
        // }
        // centers[i] = u;
      }
    }
    centers = C;
  }  // while
}

// The local search algorithm for k-means with no outliers
void weighted_local_search(const vector<arma::vec>& U, const arma::vec& weights,
                           vector<arma::vec>& C, const double eps) {
  // TODO: add some check
  const int k = C.size();
  double alpha = 1e30;

  vector<double> dists;
  vector<int> labels;
  util::compute_nearest(U, C, labels, dists);

  double cost = sum2(dists, weights);
  while (alpha * (1 - eps) > cost) {
    alpha = cost;
    auto CC = C;
    for (const auto& u : U) {
      for (int i = 0; i < k; ++i) {
        if (try_swap(U, weights, CC, i, u, dists, labels)) {
          cost = sum2(dists, weights);
        }
      }
    }
    C = CC;
  }  // while
}

void LocalSearchAlgo::perform(const ClusteringInput& input,
                              ClusteringOutput& output) {
  const double eps = 0.1;  // TODO: make this configurable

  const auto& U = input.data;
  const int k = input.n_centers;
  const int z = input.n_outliers;

  // set random points as initial centers
  auto&& C = util::subset(U, util::sample_without_repl(U.size(), k));

  auto&& Z = outliers(U, C, {}, z);
  double alpha = 1e30;

  while (alpha * (1 - eps / k) > cost_outliers(U, C, Z)) {
    alpha = cost_outliers(U, C, Z);
    //    cerr << "INFO: |Z| = " << Z.size() << endl;
    // local search with no outliers
    local_search(util::backslash(U, Z), C, eps);
    Z = outliers(U, C, {}, Z.size());

    // make a copy
    auto CC = C;
    auto ZZ = Z;

    // cost of discarding z additional outliers
    auto&& tmp_Z = util::union_sets(Z, outliers(U, C, Z, z));

    if (cost_outliers(U, C, Z) * (1 - eps / k) > cost_outliers(U, C, tmp_Z)) {
      ZZ = tmp_Z;
    }
    // for each center and non-center, perform a swap
    // and discard additional outliers
    for (const auto& u : U) {
      for (int i = 0; i < C.size(); ++i) {
        const auto v = C[i];
        C[i] = u;
        auto tmp_Z = util::union_sets(Z, outliers(U, C, {}, z));
        if (cost_outliers(U, C, tmp_Z) < cost_outliers(U, CC, ZZ)) {
          CC = C;
          ZZ = tmp_Z;
        }
        C[i] = v;  // recover C
      }
    }  // for

    // update the solution allowing additional outliers if the solution value
    // improved significantly
    if (cost_outliers(U, C, Z) * (1 - eps / k) > cost_outliers(U, CC, ZZ)) {
      C = CC;
      Z = ZZ;
    }
    //    cerr << "INFO: cost = " << cost_outliers(U, C, Z) << endl;
  }  // while

  output.centers = C;
  output.make_sure_labels_weights_outliers(input);
  output.outliers_indices = Z;
  // TODO: when z == 0, what will happen?
}


vector<int> outliers(const vector<arma::vec>& U, const vector<arma::vec>& C,
                     const arma::vec& weights, const int z,
                     vector<double>& Ow) {
  vector<double> dists;
  vector<int> labels;
  util::compute_nearest(U, C, labels, dists);
  return util::weighted_outliers(dists, weights, z, Ow);
}

double cost2(const vector<double>& dists, const arma::vec& weights,
             const int n_outliers) {
  vector<double> Ow;
  auto&& O = util::weighted_outliers(dists, weights, n_outliers, Ow);
  unordered_map<int, double> o_w;
  for (size_t i = 0; i < O.size(); ++i) {
    o_w[O[i]] = Ow[i];
  }
  double c = 0;
  for (size_t i = 0; i < dists.size(); ++i) {
    if (weights[i] < o_w[i]) {
      throw runtime_error("cost2: weights[i] < o_w[i]");
    }
    c += (weights[i] - o_w[i]) * dists[i] * dists[i];
  }
  return c;
}

double cost2(const vector<arma::vec>& U, const arma::vec& weights,
             const vector<arma::vec>& C, const int n_outliers) {
  vector<double> Ow;
  vector<double> dists;
  vector<int> labels;
  util::compute_nearest(U, C, labels, dists);
  return cost2(dists, weights, n_outliers);
}

// // Select the best swap,
// // return idx_U and idx_C
// pair<int, int> best_swap(const vector<arma::vec>& U,
//                          const arma::vec& weights,
//                          const vector<arma::vec>& C,
//                          const int z,
//                          const int cur_z) {
//   vector<double> dists;
//   vector<int> labels;
//   util::compute_nearest(U, C, labels, dists);
//   double min_cost = cost2(dists, weights, cur_z);
//   int idx_U = -1;
//   int idx_C = -1;
//   for (sizt_t i = 0; i < U.size(); ++i) {
//     for (size_t j = 0; j < C.size(); ++j) {
//       if (try_swap(...)) {
//         // TODO
//       }
//     }
//   }
// }

void WeightedLocalSearchAlgo::perform(const ClusteringInput& input,
                                      ClusteringOutput& output) {
  // TODO: add more check
  // validate the input
  if (input.data.size() != input.weights.size()) {
    throw runtime_error("WeightedLocalSearchAlgo: invalid weights");
  }

  const double eps = this->eps_;  // make this configurable

  vector<double> Ow;  // weights for outliers

  const auto& weights = input.weights;
  const auto& U = input.data;
  const int k = input.n_centers;
  const int z = input.n_outliers;

  //  auto&& C = util::subset(U,
  //                          weighted_sample_without_repl(weights, k));
  auto&& C = util::subset(U, util::sample_without_repl(U.size(), k));

  if (C.size() != k) {
    throw runtime_error("|C| != k");
  }

  double alpha = 1e30;
  int cur_z = z;  // the total weight of current Z

  int n_iters = 0;

  while (n_iters++ < max_iters_ &&
         alpha * (1 - eps) > cost2(U, weights, C, cur_z)) {
    alpha = cost2(U, weights, C, cur_z);
    if(verbose)cerr << "INFO: alpha = " << alpha << endl;
    // local search with no outliers

    // do local search without outliers
    vector<double> Ow;
    auto&& O = outliers(U, C, weights, cur_z, Ow);
    auto w2 = weights;
    //    cerr << O.size() << " " << Ow.size() << " " << w2.size() << endl;
    for (size_t i = 0; i < Ow.size(); ++i) {
      w2[O[i]] -= Ow[i];
    }
 if(verbose)   cerr << "INFO: running local search without outliers ..." << endl;
    weighted_local_search(U, w2, C, eps);
 if(verbose)   cerr << "INFO: done" << endl;

    int zz = cur_z;  // size of Z_bar in the papaer
    if (cost2(U, weights, C, cur_z) * (1 - eps) >
        cost2(U, weights, C, cur_z + z)) {
      zz = cur_z + z;
    }

    // TODO: speed it up
    // try to swap u \in U and v \in C
    arma::vec rec_u;
    int idx = -1;
    double min_cost = cost2(U, weights, C, zz);
 if(verbose)   cerr << "INFO: before swap: min_cost = " << min_cost << endl;
    for (const auto& u : U) {
      for (size_t i = 0; i < k; ++i) {
        const auto v = C[i];
        C[i] = u;
        double tmp_cost = cost2(U, weights, C, cur_z + z);
        if (tmp_cost < min_cost) {
          min_cost = tmp_cost;
          zz = cur_z + z;
          idx = i;
          rec_u = u;
        }
        C[i] = v;  // recover
      }            // for k
    }
 if(verbose)   cerr << "INFO: after_swap: min_cost = " << min_cost << endl;
    // update C and the outlier set if the improvement is significant
    if (idx >= 0 && cost2(U, weights, C, cur_z) * (1 - eps) > min_cost) {
      C[idx] = rec_u;
      cur_z = zz;
    }
  }  // while
  output.centers = C;
}

void FLSAlgo::perform(const ClusteringInput& input, ClusteringOutput& output) {
  ClusteringInput newInput(input.data,
                           alpha * (input.n_centers + input.n_outliers), 0);
  ClusteringOutput intermediateOutput;

  Kmeanspp compressionAlgo;
  compressionAlgo.perform(newInput, intermediateOutput);
  intermediateOutput.fillExtraInfo(newInput);

  ClusteringInput intermediateInput(intermediateOutput.centers, input.n_centers,
                                    input.n_outliers);
  intermediateInput.weights = intermediateOutput.weights;

  WeightedLocalSearchAlgo wls;
  wls.setMaxIters(max_iters_);
  wls.perform(intermediateInput, output);
  output.num_intermediate_centers = intermediateInput.getNrows();
}
