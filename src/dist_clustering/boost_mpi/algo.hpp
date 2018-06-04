#ifndef _ALGO_HPP_
#define _ALGO_HPP_


#include "../algorithms/util.h"
#include "../algorithms/algo_util.h"
#include "../algorithms/centeralizedMultiRoundSampling.h"
#include "../algorithms/succ_sampling.h"
#include "../algorithms/kmean_plus_plus.h"
#include "../algorithms/kmean_minus_minus.h"

#include "util.hpp"

#include <boost/mpi.hpp>
#include <boost/mpi/collectives.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <map>
#include <exception>
#include <memory>
#include <cstdlib>
#include <boost/serialization/string.hpp>


namespace mpi = boost::mpi;
using namespace std;

// list of global constants
const int ROOT = 0;

class CoresetFunc {
public:
  virtual arma_vectors operator ()(const vector<arma::vec>& U, int k, int t) {};
};

arma_vectors bg_func(const vector<arma::vec>& U, int k, int t) {
  SuccSampling2 bg;
  //TODO: tune those parameters
  bg.setAlpha(1);
  bg.setBeta(0.3);
  bg.setGamma(5);
  ClusteringInput input;
  input.data = U;
  input.n_centers = k;
  input.n_outliers = t;
  ClusteringOutput output;
  bg.perform(input, output);
  auto& C = output.centers;
  auto& indices = output.centers_indices;
  for (int i : output.outliers_indices) {
    C.push_back(U[i]);
    indices.push_back(i);
  }
  return arma_vectors(C, indices);
}


arma_vectors bg_func(const arma_vectors& U, int k, int t) {
  SuccSampling2 bg;
  //TODO: tune those parameters
  bg.setAlpha(2);
  bg.setBeta(4.5);
  bg.setGamma(5);
 
  ClusteringInput input;
  input.data = to_vecs(U);
  input.n_centers = k;
  input.n_outliers = t;
  ClusteringOutput output;
  bg.perform(input, output);

  arma_vectors res;
  vector<double> ws;
  int i = 0;
  for (const int idx : output.centers_indices) {
    res.data.push_back(U.data[idx]);
    ws.push_back(output.weights[i]);
    ++i;
  }
  
  for (const int idx : output.outliers_indices) {
    res.data.push_back(U.data[idx]);
    ws.push_back(1);
  }
  res.weights = arma_vec(ws);
  return res;
}


arma_vectors kmmm_func(const arma_vectors& U, int k, int coreset_size) {
  KMeanMinusMinus algo;
  algo.setMaxIter(1000);
  algo.setThresh(0.01);
  ClusteringInput input;
  input.data = to_vecs(U);
  input.n_centers = k;
  input.n_outliers = coreset_size / 2;

  ClusteringOutput output;
  algo.perform(input, output);
  arma_vectors res;
  vector<double> ws;
  // first add outliers
  for (const int idx : output.outliers_indices) {
    res.data.push_back(U.data[idx]);
    ws.push_back(1);
  }

  
  vector<int> labels;
  vector<double> dists;
  util::compute_nearest(output.centers, input.data, labels, dists);

  
  auto&& extra_idx = util::sample_with_repl(U.data.size(), coreset_size - input.n_outliers - k);

  
  extra_idx.insert(extra_idx.end(), labels.begin(), labels.end());


  if (extra_idx.size() != coreset_size - res.data.size()) {
    throw runtime_error("!!!");
  }
  vector<arma::vec> extra_vecs;
  for (int i : extra_idx) {
    extra_vecs.push_back(input.data[i]);
  }
  
  util::remove_subset(input.data, output.outliers_indices);
  auto&& extra_ws = util::compute_weights(input.data, extra_vecs);


  for (size_t i = 0; i < extra_vecs.size(); ++i) {
    ws.push_back(extra_ws[i]);
    res.data.push_back(U.data[extra_idx[i]]);
  }
  
  res.weights = arma_vec(ws);
  if (res.data.size() != coreset_size) {
    cerr << "|res| = " << res.data.size() << " coreset_size = " << coreset_size << endl; 
    throw runtime_error("kmmm_func: final size error");
  }
  return res;
}




arma_vectors km_func(const arma_vectors& U, const int coreset_size) {
  ClusteringInput input;
  input.data = to_vecs(U);
  input.n_centers = coreset_size;

  ClusteringOutput output;
  KMeanPP algo;
  algo.perform(input, output);

  arma_vectors res;
  vector<double> ws;
  int i = 0;
  for (const int idx : output.centers_indices) {
    res.data.push_back(U.data[idx]);
    ws.push_back(output.weights[i]);
    ++i;
  }
  res.weights = arma_vec(ws);
  return res;
}


arma_vectors rand_func(const arma_vectors& U, int n) {
  arma_vectors res;
  auto&& idx = util::sample_without_repl(U.data.size(), n);
  res.data = util::subset(U.data, idx);
  res.weights = arma_vec(compute_weights(to_vecs(U), to_vecs(res)));
  return res;
}


arma_vectors rand_func(const vector<arma::vec>& U, int k, int t) {
  return arma_vectors(util::subset(U,
				   util::sample_without_repl(U.size(),
							     k + t)));
}



// an even newer version, this version only compute weights
// locally. This version will also return the indices
class AlgoMPI3 {
public:
  void perform(const mpi::communicator& world,
	       const int root,
	       const arma_vectors& U,
	       arma_vectors& coreset,
	       const string& algo_name,
	       const int k,
	       const int t) {
    int coreset_size = 0;
    int outliers_size = 0;
    if (world.rank() != root) {
      coreset_size = k / world.size();
      outliers_size = t / world.size();
    } else {
      coreset_size = k - (world.size() - 1) * (k / world.size());
      outliers_size = t - (world.size() - 1) * (t / world.size());
    }
    if (coreset_size < 1) {
      coreset_size = 1;
    }
    // locally construct coreset
    arma_vectors local_set;
    if (algo_name == "rand") {
      local_set = rand_func(U, coreset_size);
    } else if (algo_name == "km") {
      local_set = km_func(U, coreset_size);
    } else if (algo_name == "kmean--") {
      local_set = kmmm_func(U, k, outliers_size);
    } else {
      //      local_set = bg_func(U, coreset_size, outliers_size);
      local_set = bg_func(U, k, outliers_size);
    }


    // compute local weights and send to root
    //    local_set.weights.data = compute_weights(to_vecs(U), to_vecs(local_set));
        
    vector<arma_vectors> all_sets;
    if (world.rank() == root) {
      mpi::gather(world, local_set, all_sets, root);
    } else {
      mpi::gather(world, local_set, root);
    }

    // aggregate local_set/weights in root
    if (world.rank() == root) {
      coreset.clear();
      vector<double>& ws = coreset.weights.data;
      for (const auto& v : all_sets) {
	coreset.data.insert(coreset.data.end(), v.data.begin(), v.data.end());
	ws.insert(ws.end(), v.weights.data.begin(), v.weights.data.end());
      }
    }
    if (coreset.weights.data.size() != coreset.data.size()) {
      runtime_error("AlgoMPI3: |weights| != |coreset|");
    }
  }
};


// this version computes weights
// locally. This version will also return the indices
class AlgoMPI4 {
public:
  void perform(const mpi::communicator& world,
	       const int root,
	       const arma_vectors& U,
	       arma_vectors& coreset,
	       const string& algo_name,
	       const int k,
	       const int t) {
    int coreset_size = 0;
    int outliers_size = 0;
    if (world.rank() != root) {
      coreset_size = k / world.size();
      outliers_size = t / world.size();
    } else {
      coreset_size = k - (world.size() - 1) * (k / world.size());
      outliers_size = t - (world.size() - 1) * (t / world.size());
    }
    // locally construct coreset
    arma_vectors local_set;
    if (algo_name == "rand") {
      local_set = rand_func(U, coreset_size);
    } else {
      local_set = bg_func(U, coreset_size, outliers_size);
    }


    // compute local weights and send to root
    //    local_set.weights.data = compute_weights(to_vecs(U), to_vecs(local_set));
        
    vector<arma_vectors> all_sets;
    if (world.rank() == root) {
      mpi::gather(world, local_set, all_sets, root);
    } else {
      mpi::gather(world, local_set, root);
    }


    mpi::broadcast(world, all_sets, root);

    // merge all points sampled in different sites
    coreset.clear();
    vector<double>& ws = coreset.weights.data;
    for (const auto& v : all_sets) {
      coreset.data.insert(coreset.data.end(), v.data.begin(), v.data.end());
      //      ws.insert(ws.end(), v.weights.data.begin(), v.weights.data.end());
    }
    
    // compute weights locally and send to root
    coreset.weights.data = compute_weights(to_vecs(U), to_vecs(coreset));
    if (coreset.weights.data.size() != coreset.data.size()) {
      runtime_error("AlgoMPI4: |weights| != |coreset|");
    }
    vector<arma_vec> all_weights;
    if (world.rank() == root) {
      mpi::gather(world, coreset.weights, all_weights, root);
    } else {
      mpi::gather(world, coreset.weights, root);
    }

    // aggregate weights in root
    if (world.rank() == root) {
      arma::vec&& ws = arma::vec(all_weights[0].data);
      for (int i = 1; i < all_weights.size(); ++i) {
	ws += arma::vec(all_weights[i].data);
      }
      coreset.weights.data = util::arma_vec_to_vector(ws);
    }
  }
};





// new version, this version only compute weights
// locally.
class AlgoMPI2 {
public:
  void perform(const mpi::communicator& world,
	       const int root,
	       const vector<arma::vec>& U,
	       vector<arma::vec>& coreset,
	       arma::vec& weights,
	       const string& algo_name,
	       const int k,
	       const int t) {
    int coreset_size = 0;
    int outliers_size = 0;
    if (world.rank() != root) {
      coreset_size = k / world.size();
      outliers_size = t / world.size();
    } else {
      coreset_size = k - (world.size() - 1) * (k / world.size());
      outliers_size = t - (world.size() - 1) * (t / world.size());
    }
    // locally construct coreset
    arma_vectors local_set;
    if (algo_name == "rand") {
      local_set = rand_func(U, coreset_size, outliers_size);
    } else {
      local_set = bg_func(U, coreset_size, outliers_size);
    }

    coreset = to_vecs(local_set);    
    // compute local weights and send to root
    local_set.weights.data = compute_weights(U, coreset);
        
    vector<arma_vectors> all_sets;
    if (world.rank() == root) {
      mpi::gather(world, local_set, all_sets, root);
    } else {
      mpi::gather(world, local_set, root);
    }

    // aggregate local_set/weights in root
    if (world.rank() == root) {
      vector<double> ws;
      coreset.clear();
      for (const auto& v : all_sets) {
	auto&& ss = to_vecs(v);
	coreset.insert(coreset.end(), ss.begin(), ss.end());
	ws.insert(ws.end(), v.weights.data.begin(), v.weights.data.end());
      }
      weights = arma::vec(ws);
    }
    if (weights.size() != coreset.size()) {
      runtime_error("AlgoMPI2: |weights| != |coreset|");
    }
  }
};

class AlgoMPI {
public:
  void perform(const mpi::communicator& world,
	       const int root,
	       const vector<arma::vec>& U,
	       vector<arma::vec>& coreset,
	       arma::vec& weights,
	       const string& algo_name,
	       const int k,
	       const int t) {
    int coreset_size = 0;
    int outliers_size = 0;
    if (world.rank() != root) {
      coreset_size = k / world.size();
      outliers_size = t / world.size();
    } else {
      coreset_size = k - (world.size() - 1) * (k / world.size());
      outliers_size = t - (world.size() - 1) * (t / world.size());
    }
    // locally construct coreset
    arma_vectors local_set;
    if (algo_name == "rand") {
      local_set = rand_func(U, coreset_size, outliers_size);
    } else {
      local_set = bg_func(U, coreset_size, outliers_size);
    }
    
    vector<arma_vectors> all_sets;
    if (world.rank() == root) {
      mpi::gather(world, local_set, all_sets, root);
    } else {
      mpi::gather(world, local_set, root);
    }
    
    mpi::broadcast(world, all_sets, root);
    // merge all sampled points as the coreset, this happens in each
    // process
    coreset.clear();
    for (const auto& s : all_sets) {
      auto&& ss = to_vecs(s);
      coreset.insert(coreset.end(), ss.begin(), ss.end());
    }
    
    // compute local weights and send to root
    arma_vec ws;
    ws.data = compute_weights(U, coreset);
    vector<arma_vec> all_weights;
    if (world.rank() == root) {
      mpi::gather(world, ws, all_weights, root);
    } else {
      mpi::gather(world, ws, root);
    }

    // aggregate weights in root
    if (world.rank() == root) {
      weights = arma::vec(all_weights[0].data);
      for (int i = 1; i < all_weights.size(); ++i) {
	weights += arma::vec(all_weights[i].data);
      }
    }
  }

private:
  CoresetFunc func_;
};


void kmeanpp(const vector<arma::vec>& U,
             const arma::vec& weights,
             const int n_centers,
             vector<arma::vec>& coreset,
             arma::vec& ws) {
  Kmeanspp kmpp;
  ClusteringInput input;
  input.data = U;
  input.weights = weights;
  input.n_centers = n_centers;  
  ClusteringOutput output;
  kmpp.perform(input, output);
  output.make_sure_labels_weights_outliers(input);
  coreset = output.centers;
  ws = output.weights;
}

void kmean_par(const mpi::communicator& world,
	       const int root,
	       const vector<arma::vec>& U,
	       vector<arma::vec>& coreset,
	       arma::vec& weights,
	       const int k,
	       int& msg_size) {
  msg_size = 0;
  const int n_rounds = 6;
  const double ell = k;
  arma_vectors C;  // centers
  C.data.push_back(arma_vec(U[0]));  // pick a initial center 

  // initialize a uniform distribution
  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0, 1.0);

  
  for (int r = 0; r < n_rounds; ++r) {
    if (world.rank() != root) {
      C.data.clear();
    }
    
    mpi::broadcast(world, C, root);
    msg_size += C.data.size();
    const auto&& CC = to_vecs(C);

    // compute dists
    vector<double> dists;
    vector<int> labels;
    util::compute_nearest(U, CC, labels, dists);
    double phi_local = util::sum2(dists);
    double phi = -1;
    
    if (world.rank() == root) {
      vector<double> phis;
      mpi::gather(world, phi_local, phis, root);
      phi = util::sum(phis);
    } else {
      mpi::gather(world, phi_local, root);
    }
    mpi::broadcast(world, phi, root);


    // each processor has the phi, now will perform sampling
    arma_vectors new_centers;
    for (size_t i = 0; i < U.size(); ++i) {
      double rd = dis(gen);
      if (rd < ell * dists[i] * dists[i] / phi) {
        new_centers.data.push_back(arma_vec(U[i]));
      }
    }

    // gather all new centers
    if (world.rank() == root) {
      vector<arma_vectors> vec_new_centers;
      mpi::gather(world, new_centers, vec_new_centers, root);
      for (const auto& nc : vec_new_centers) {
        C.data.insert(C.data.end(), nc.data.begin(), nc.data.end());
      }
    } else {
      mpi::gather(world, new_centers, root);
    }
  }


  // broadcast the final coreset
  if (world.rank() != root) {
    C.data.clear();
  }
  mpi::broadcast(world, C, root);
  coreset = to_vecs(C);
  
  // compute local weights and send to root
  arma_vec ws;
  ws.data = compute_weights(U, coreset);
  vector<arma_vec> all_weights;
  if (world.rank() == root) {
    mpi::gather(world, ws, all_weights, root);
  } else {
    mpi::gather(world, ws, root);
  }
  
  // aggregate weights in root
  if (world.rank() == root) {
    weights = arma::vec(all_weights[0].data);
    for (int i = 1; i < all_weights.size(); ++i) {
      weights += arma::vec(all_weights[i].data);
    }
  }
}




void mpi_kmean(const mpi::communicator& world,
	       const int root,
	       const arma_vectors& U,
	       arma_vectors& coreset,
	       const int k,
	       int& msg_size) {
  msg_size = 0;
  const int n_rounds = 5;
  const double ell = k;
  arma_vectors C;  // centers
  C.data.push_back(U.data[0]);  // pick a initial center 

  // initialize a uniform distribution
  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0, 1.0);

  auto&& UU = to_vecs(U);
  // compute dists
  vector<double> dists;
  vector<int> labels;
  util::compute_nearest(UU, to_vecs(C), labels, dists);

  arma_vectors new_centers = C;
  for (int r = 0; r < n_rounds; ++r) {
    if (world.rank() != root) {
      new_centers.clear();
    }
    
    mpi::broadcast(world, new_centers, root);
    msg_size += new_centers.data.size();
    const auto&& CC = to_vecs(new_centers);

    // update dists based on new_centers
    vector<double> new_dists;
    vector<int> new_labels;
    util::compute_nearest(UU, CC, new_labels, new_dists);
    for (size_t i = 0; i < UU.size(); ++i) {
      if (new_dists[i] < dists[i]) {
	dists[i] = new_dists[i];
      }
    }
    double phi_local = util::sum2(dists);
    double phi = -1;
    
    if (world.rank() == root) {
      vector<double> phis;
      mpi::gather(world, phi_local, phis, root);
      phi = util::sum(phis);
    } else {
      mpi::gather(world, phi_local, root);
    }
    mpi::broadcast(world, phi, root);


    // each processor has the phi, now will perform sampling
    //    arma_vectors new_centers;
    new_centers.clear();
    for (size_t i = 0; i < U.data.size(); ++i) {
      double rd = dis(gen);
      if (rd < ell * dists[i] * dists[i] / phi) {
        new_centers.data.push_back(U.data[i]);
      }
    }

    // gather all new centers
    if (world.rank() == root) {
      vector<arma_vectors> vec_new_centers;
      mpi::gather(world, new_centers, vec_new_centers, root);
      new_centers.clear();
      for (const auto& nc : vec_new_centers) {
        C.data.insert(C.data.end(), nc.data.begin(), nc.data.end());
	new_centers.data.insert(new_centers.data.end(), nc.data.begin(), nc.data.end());
      }
    } else {
      mpi::gather(world, new_centers, root);
    }
  }


  // broadcast the final coreset
  if (world.rank() != root) {
    C.data.clear();
  }
  mpi::broadcast(world, C, root);
  coreset = C;
  
  // compute local weights and send to root
  arma_vec ws;
  ws.data = compute_weights(to_vecs(U), to_vecs(coreset));
  vector<arma_vec> all_weights;
  if (world.rank() == root) {
    mpi::gather(world, ws, all_weights, root);
  } else {
    mpi::gather(world, ws, root);
  }
  
  // aggregate weights in root
  if (world.rank() == root) {
    auto&& weights = arma::vec(all_weights[0].data);
    for (int i = 1; i < all_weights.size(); ++i) {
      weights += arma::vec(all_weights[i].data);
    }
    coreset.weights = arma_vec(weights);
  }
}




#endif
