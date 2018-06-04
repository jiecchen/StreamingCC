#include <iostream>
#include "../algorithms/kmean_minus_minus.h"
#include "../algorithms/simple_algo.h"
#include "../algorithms/succ_sampling.h"
#include "../algorithms/util.h"
#include "../experiments/dataManager.h"

#include <set>
#include <vector>
using namespace std;

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

double calc_cost1(const vector<double>& dists, const int n_outliers) {
  double c = 0;
  auto ot = util::indices_of_upper(dists, n_outliers);
  set<int> O(ot.begin(), ot.end());
  for (int i = 0; i < dists.size(); ++i) {
    if (O.count(i) == 0) {
      c += dists[i];
    }
  }
  return c;
}

double calc_cost2(const vector<double>& dists, const int n_outliers) {
  double c = 0;
  auto ot = util::indices_of_upper(dists, n_outliers);
  set<int> O(ot.begin(), ot.end());
  for (int i = 0; i < dists.size(); ++i) {
    if (O.count(i) == 0) {
      c += dists[i] * dists[i];
    }
  }
  return c;
}

pair<double, double> run_algo_internal(ClusteringAlgo* algo,
                                       const ClusteringInput& input,
                                       const vector<arma::vec>& dataset) {
  ClusteringOutput output;
  algo->perform(input, output);
  vector<int> labels;
  vector<double> dists;
  util::compute_nearest(dataset, output.centers, labels, dists);
  return {calc_cost1(dists, input.n_outliers),
          calc_cost2(dists, input.n_outliers)};
}

pair<double, double> run_algo(ClusteringAlgo* algo,
                              const ClusteringInput& input,
                              const vector<arma::vec>& dataset) {
  double l1 = 0;
  double l2 = 0;
  const int N = 1;
  for (int i = 0; i < N; ++i) {
    auto res = run_algo_internal(algo, input, dataset);
    l1 += res.first;
    l2 += res.second;
  }
  return {l1 / N, l2 / N};
}

void run_test() {
  ClusteringInput input;
  DataSet dataset;
  dataset.name = "kddcup";
  dataset.folderName = "/home/shared/distAlgData/";
  dataset.load();

  input.n_centers = 20;
  input.n_outliers = 100;
  input.data = dataset.data;

  //   ArmaKmeansClustering kmean;
  //   kmean.n_iter = 100;
  //   auto res = run_algo(&kmean, input, input.data);
  //   cout << "kmean: " << res.first << " " << res.second << endl;

  //   RandomClustering rnd;
  //   auto res = run_algo(&rnd, input, input.data);
  //   cout << "rand: " << res.first << " " << res.second << endl;

  SuccSampling succ;
  succ.setAlpha(2.);
  succ.setBeta(0.3);
  auto res = run_algo(&succ, input, input.data);
  cout << "succ: " << res.first << " " << res.second << endl;

  //   KMeanMinusMinus kmmm;
  //   res = run_algo(&kmmm, input, input.data);
  //   cout << "kmmm: " << res.first << " " << res.second << endl;
  /*
     // construct the input for weighted_kmean--
     auto input_ = input;
     ClusteringOutput output;
     succ.perform(input, output);
     input_.data = output.centers;
     vector<double> wts;
     for (int i = 0; i < output.weights.size(); ++i) {
       wts.push_back(output.weights[i]);
     }
     for (int i : output.outliers_indices) {
       input_.data.push_back(input.data[i]);
       wts.push_back(1);
     }
     input_.weights = arma::vec(wts);


     WeightedKMeanMM wkmmm;
     res = run_algo(&wkmmm, input_, input.data);
     cout << "wkmmm: " << res.first << " " << res.second << endl;

     res = run_algo(&kmmm, input_, input.data);
     cout << "kmmm_new: " << res.first << " " << res.second << endl;
    */
}

int main() {
  run_test();
  return 0;
}
