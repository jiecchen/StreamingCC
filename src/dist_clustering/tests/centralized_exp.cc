#include <iostream>
#include "../algorithms/centeralizedMultiRoundSampling.h"
#include "../algorithms/kmean_minus_minus.h"
#include "../algorithms/local_search.h"
#include "../algorithms/simple_algo.h"
#include "../algorithms/succ_sampling.h"
#include "../algorithms/timer.h"
#include "../algorithms/util.h"
#include "../experiments/dataManager.h"

#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

const int N = 1;  // # of repeats

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

class ResType {
 public:
  ResType() : l1(0), l2(0), tm(0){};
  double l1;
  double l2;
  double tm;
};

ResType run_algo_internal(ClusteringAlgo* algo, const ClusteringInput& input,
                          const vector<arma::vec>& dataset) {
  ClusteringOutput output;
  Timer timer;
  algo->perform(input, output);
  double tm = timer.get_time();
  vector<int> labels;
  vector<double> dists;
  util::compute_nearest(dataset, output.centers, labels, dists);
  ResType res;
  res.l1 = calc_cost1(dists, input.n_outliers);
  res.l2 = calc_cost2(dists, input.n_outliers);
  res.tm = tm;
  return res;
}

ResType run_algo(ClusteringAlgo* algo, const ClusteringInput& input,
                 const vector<arma::vec>& dataset) {
  double l1 = 0;
  double l2 = 0;
  double tm = 0;
  for (int i = 0; i < N; ++i) {
    auto res = run_algo_internal(algo, input, dataset);
    l1 += res.l1;
    l2 += res.l2;
    tm += res.tm;
  }
  ResType res;
  res.l2 = l2 / N;
  res.l1 = l1 / N;
  res.tm = tm / N;
  return res;
}

string to_string(const ResType& res) {
  stringstream ss;
  ss << "l1: " << res.l1 << ", l2: " << res.l2 << ", time: " << res.tm;
  return ss.str();
}

ClusteringInput add_unit_weights(const ClusteringInput& input) {
  auto copy = input;
  vector<double> wts(input.data.size(), 1);
  copy.weights = arma::vec(wts);
  return copy;
}

void run_test() {
  ClusteringInput input;
  DataSet dataset;
  dataset.name = "spambase";
  //   dataset.folderName="/home/shared/distAlgData/";
  dataset.load();

  input.n_centers = 20;
  input.n_outliers = 100;
  input.data = util::subset(
      dataset.data, util::sample_without_repl(dataset.data.size(), 1000));

  RandomClustering rnd;
  auto res = run_algo(&rnd, input, input.data);
  cout << "rand: " << to_string(res) << endl;

  SuccSampling succ;
  succ.setAlpha(2.);
  succ.setBeta(0.3);
  res = run_algo(&succ, input, input.data);
  cout << "succ: " << to_string(res) << endl;

  Kmeanspp kmpp;
  res = run_algo(&kmpp, input, input.data);
  cout << "kmpp: " << to_string(res) << endl;

  KMeanMinusMinus kmmm;
  res = run_algo(&kmmm, input, input.data);
  cout << "kmmm: " << to_string(res) << endl;

  WeightedLocalSearchAlgo wls;
  wls.setEps(0.1);
  wls.setMaxIters(2);
  auto&& input1 = add_unit_weights(input);
  res = run_algo(&wls, input1, input.data);
  cout << "wls: " << to_string(res) << endl;

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
