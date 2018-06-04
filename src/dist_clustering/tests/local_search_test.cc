#include "../algorithms/local_search.h"
#include "../algorithms/centeralizedMultiRoundSampling.h"
#include "../algorithms/simple_algo.h"
#include "../algorithms/util.h"
#include "../experiments/dataManager.h"

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../lest.hpp"

#include <iostream>
#include <vector>

using namespace std;

// TODO: add test for SuccSampling

const lest::test specification[] = {

    CASE("cost2"){vector<double> tmp{1, 2, 3, 4, 5, 6, 7};
vector<arma::vec> U;
for (double a : tmp) {
  U.push_back(arma::vec{a});
}
vector<arma::vec> C{arma::vec{1}, arma::vec{2}};
EXPECT(cost2(U, C, 5) == 0);
EXPECT(cost2(U, C, 4) == 1);
EXPECT(cost2(U, C, 3) == 5);
EXPECT(cost2(U, C, 2) == 14);
}
,

    CASE("local_search") {
  DataSet dataSet;
  dataSet.name = "kdd200";
  dataSet.load();
  auto& U = dataSet.data;
  const int n_outliers = 0;
  const int n_centers = 5;
  auto&& idx = util::sample_without_repl(U.size(), n_centers);
  vector<arma::vec> C = util::subset(U, idx);

  double old_cost = cost2(U, C, n_outliers);
  for (int i = 0; i < 10; ++i) {
    cerr << "INFO: cost = " << old_cost << endl;
    local_search(U, C);
    double new_cost = cost2(U, C, n_outliers);
    EXPECT(old_cost >= new_cost);
    old_cost = new_cost;
  }
  cerr << "INFO: cost = " << old_cost << endl;
}
,

    CASE("LocalSearchAlgo") {
  ClusteringInput input;
  ClusteringOutput output;
  DataSet dataSet;
  dataSet.name = "kdd200";
  dataSet.load();
  input.n_centers = 4;
  input.n_outliers = 1;
  input.data = dataSet.data;

  LocalSearchAlgo algo;
  algo.perform(input, output);
  EXPECT(output.centers.size() == input.n_centers);

  double c1 = cost2(input.data, output.centers,
                    input.n_outliers);  // output.outliers_indices.size());

  local_search(input.data, output.centers);
  double c3 = cost2(input.data, output.centers, input.n_outliers);

  Kmeanspp kmpp;
  ClusteringOutput output2;
  kmpp.perform(input, output2);
  double c2 = cost2(input.data, output2.centers, input.n_outliers);

  RandomClustering rnd;
  ClusteringOutput output4;
  rnd.perform(input, output4);
  double c4 = cost2(input.data, output4.centers, input.n_outliers);

  cout << "local_search = " << c3 << endl;
  cout << "LocalSearchAlgo = " << c1 << endl;
  cout << "kmean++ = " << c2 << endl;
  cout << "random = " << c4 << endl;
}
, CASE("FLS") {
  ClusteringInput input;
  ClusteringOutput output;
  DataSet dataSet;
  dataSet.name = "spambase";
  dataSet.load();
  input.n_centers = 20;
  input.n_outliers = 100;
  input.data = dataSet.data;

  FLSAlgo algo;
  algo.perform(input, output);
  EXPECT(output.centers.size() == input.n_centers);
}
}
;

int main() { return lest::run(specification); }
