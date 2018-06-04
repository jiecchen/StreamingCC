// The implementation of Successive Sampling

#include "succ_sampling.h"
#include "util.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

void SuccSampling::perform(const ClusteringInput& input,
                           ClusteringOutput& output) {
  double alpha = this->alpha_;
  double beta = this->beta_;

  // clear up output
  output.centers.clear();
  output.outliers_indices.clear();

  if(verbose)  cerr << "INFO: alpha = " << alpha << ", beta = " << beta << endl;
  if(verbose)  cerr << "INFO: # of points = " << input.data.size() << endl;

  // make a copy of data
  auto U = input.data;
  // to keep the original indices of the remaining points
  vector<int> indices;

  // to keep the weights for centers
  vector<double> weights;

  for (int i = 0; i < U.size(); ++i) {
    indices.push_back(i);
  }

  //  const int n_samples = ceil(alpha * (input.n_centers + log2(U.size())));
  int n_samples = ceil(alpha * input.n_centers);
  if(verbose) cerr << "INFO: n_samples = " << n_samples << endl;

  if(verbose)  cerr << "INFO: performing sampling ..." << endl;

  int n_rounds = 0;
  while (U.size() > gamma_ * input.n_outliers) {
    // this trick will significantly improve the recall
    int tmp_n = ceil(0.02 * U.size());
    n_samples = min(n_samples, tmp_n);
    //    cerr << "INFO: Round " << ++n_rounds << " ---------------" << endl;
    //    cerr << "recall = " << util::recall(input.actual_outliers, indices) << endl;
    //    cerr << "|U| = " << indices.size() << endl;
    // sample centers from U with replacement
    // use the index to represent the corresponding points
    // use sampling without replacement so that S.size() <= U.size()
    auto&& S = util::sample_without_repl(U.size(), n_samples);

    // for each point in U, compute its nearest point in S
    vector<int> nearest_indices;
    vector<double> min_dist;
    util::compute_nearest(U, S, nearest_indices, min_dist);
    assert(U.size() == nearest_indices.size());
    assert(U.size() == min_dist.size());
    // compute the smallest \nu s.t. |B(U, S, nu)| > beta |U|
    // and construct C = B(U, S, nu)
    // use the index to represent the corresponding point
    int n_to_remove = ceil(beta * U.size() + input.n_centers);
    if (n_to_remove + 2 * input.n_outliers > U.size()) {
      n_to_remove = U.size() - 2 * input.n_outliers;
    }
    auto&& C = util::indices_of_lower(min_dist, n_to_remove);
    if(verbose)    cerr << "INFO: |C| = " << C.size() << endl;

    // calculate weight for each center
    unordered_map<int, int> map_count;
    for (int i : C) {
      map_count[nearest_indices[i]]++;
    }

    set<int> extra_outliers;
    for (const auto& p : map_count) {
      if (p.second > -1) {
    	output.centers.push_back(U[p.first]);
    	output.centers_indices.push_back(indices[p.first]);
    	weights.push_back(p.second);
      } else {
    	extra_outliers.insert(indices[p.first]);
      }
    }
    //    cerr << "|extra_outliers| = " << extra_outliers.size() << endl;
    // set extra_outliers invalid in C
    // so that they will not be removed from U and indices
    for (int& i : C) {
      if (extra_outliers.count(i) > 0) {
    	i = -1; // set an invalid number
      }
    }
    // remove C from U
    util::remove_subset<arma::vec>(U, C);
    util::remove_subset<int>(indices, C);
    
    assert(U.size() == indices.size());
  }
  
  weights = util::compute_weights(input.data, output.centers);
  output.weights = arma::vec(weights);
  output.num_intermediate_centers = output.centers.size();
  output.outliers_indices = indices;
}



void SuccSampling2::perform(const ClusteringInput& input,
                           ClusteringOutput& output) {
  double alpha = this->alpha_;
  double beta = this->beta_;

  // clear up output
  output.centers.clear();
  output.outliers_indices.clear();

  if(verbose)  cerr << "INFO: alpha = " << alpha << ", beta = " << beta << endl;
  if(verbose)  cerr << "INFO: # of points = " << input.data.size() << endl;

  // make a copy of data
  auto U = input.data;
  // to keep the original indices of the remaining points
  vector<int> indices;

  // to keep the weights for centers
  vector<double> weights;

  for (int i = 0; i < U.size(); ++i) {
    indices.push_back(i);
  }

  //  const int n_samples = ceil(alpha * (input.n_centers + log2(U.size())));
  int n_samples = ceil(alpha * input.n_centers);
  if(verbose) cerr << "INFO: n_samples = " << n_samples << endl;

  if(verbose)  cerr << "INFO: performing sampling ..." << endl;

  int n_rounds = 0;
  while (U.size() > gamma_ * input.n_outliers) {
    // this trick will significantly improve the recall
    //    int tmp_n = ceil(0.01 * U.size());
    //    n_samples = min(n_samples, tmp_n);
    //    cerr << "INFO: Round " << ++n_rounds << " ---------------" << endl;
    //    cerr << "recall = " << util::recall(input.actual_outliers, indices) << endl;
    //    cerr << "|U| = " << indices.size() << endl;
    // sample centers from U with replacement
    // use the index to represent the corresponding points
    // use sampling without replacement so that S.size() <= U.size()
    auto&& S = util::sample_without_repl(U.size(), n_samples);

    // for each point in U, compute its nearest point in S
    vector<int> nearest_indices;
    vector<double> min_dist;
    util::compute_nearest(U, S, nearest_indices, min_dist);
    assert(U.size() == nearest_indices.size());
    assert(U.size() == min_dist.size());
    // compute the smallest \nu s.t. |B(U, S, nu)| > beta |U|
    // and construct C = B(U, S, nu)
    // use the index to represent the corresponding point
    int n_to_remove = ceil(beta * U.size() + input.n_centers);
    if (n_to_remove + 2 * input.n_outliers > U.size()) {
      n_to_remove = U.size() - 2 * input.n_outliers;
    }
    auto&& C = util::indices_of_lower(min_dist, n_to_remove);
    if(verbose)    cerr << "INFO: |C| = " << C.size() << endl;



    // calculate weight for each center
    unordered_map<int, int> map_count;
    for (int i : C) {
      map_count[nearest_indices[i]]++;
    }


    set<int> extra_outliers;
    for (const auto& p : map_count) {
      if (p.second > 5) {
    	output.centers.push_back(U[p.first]);
    	output.centers_indices.push_back(indices[p.first]);
    	weights.push_back(p.second);
      } else {
    	extra_outliers.insert(indices[p.first]);
      }
    }

    // set extra_outliers invalid in C
    // so that they will not be removed from U and indices
    for (int& i : C) {
      if (extra_outliers.count(i) > 0) {
    	i = -1; // set an invalid number
      }
    }
    

    // remove C from U
    util::remove_subset<arma::vec>(U, C);
    util::remove_subset<int>(indices, C);
    
    assert(U.size() == indices.size());
  }


  vector<int> list;
  for (int i = 0; i < input.data.size(); ++i) {
    list.push_back(i);
  }

  util::remove_subset(list, indices);
  auto&& new_idx = util::sample_without_repl(list.size(), indices.size());
  for (int i : new_idx) {
    int idx = list[i];
    output.centers_indices.push_back(idx);
    output.centers.push_back(input.data[idx]);
  }
  
  weights = util::compute_weights(input.data, output.centers);
  output.weights = arma::vec(weights);
  output.num_intermediate_centers = output.centers.size();
  output.outliers_indices = indices;
}
