#include "../algorithms/algo_util.h"
#include "../algorithms/centeralizedMultiRoundSampling.h"
#include "../algorithms/local_search.h"
#include "../algorithms/simple_algo.h"
#include "../algorithms/succ_sampling.h"
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

    // CASE("weighted_local_search") {
    //   ClusteringInput input;
    //   DataSet dataSet;
    //   dataSet.name = "kdd10000";
    //   dataSet.load();
    //   input.data = dataSet.data;
    //   input.n_outliers = 10;
    //   input.n_centers = 20;

    //   ClusteringOutput output;
    //   SuccSampling succ;
    //   succ.setAlpha(2.);
    //   succ.setBeta(0.3);
    //   succ.perform(input, output);

    //   // construct input for weighted case
    //   auto input_ = input;
    //   input_.data = output.centers;
    //   vector<double> wts;
    //   for (int i = 0; i < output.weights.size(); ++i) {
    //    wts.push_back(output.weights[i]);
    //   }
    //   for (int i : output.outliers_indices) {
    //     input_.data.push_back(input.data[i]);
    //     wts.push_back(1);
    //   }
    //   input_.weights = arma::vec(wts);

    //   cerr << "INFO: |U| = " << input_.data.size() << endl;

    //   // test weighted_local_search
    //   vector<arma::vec>&& C = util::subset(input.data,
    //       				 util::sample_without_repl(input.data.size(),
    //       input.n_centers));

    //   vector<double> dists;
    //   vector<int> labels;
    //   util::compute_nearest(input_.data, C, labels, dists);
    //   double old_cost = sum2(dists, input_.weights);
    //   for (int i = 0; i < 2; ++i) {
    //     cerr << "INFO: cost = " << old_cost << endl;
    //     weighted_local_search(input_.data, input_.weights, C, 0.1);
    //     util::compute_nearest(input_.data, C, labels, dists);
    //     double new_cost = sum2(dists, input_.weights);
    //     EXPECT( old_cost >= new_cost );
    //     old_cost = new_cost;
    //   }
    //   cerr << "INFO: wls cost = " << cost2(input.data, C) << endl;

    //   ClusteringOutput opt;
    //   Kmeanspp kmpp;
    //   kmpp.perform(input, opt);
    //   cerr << "INFO: kmean++ = " << cost2(input.data, opt.centers) << endl;
    // },

    CASE("WeightedLocalSearchAlgo"){ClusteringInput input;
DataSet dataSet;
dataSet.name = "kdd10000";
dataSet.load();
input.data = dataSet.data;
input.n_outliers = 10;
input.n_centers = 20;

ClusteringOutput output;
SuccSampling succ;
succ.setAlpha(2.);
succ.setBeta(0.3);
succ.perform(input, output);

// construct input for weighted case
auto input_ = input;
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

cerr << "INFO: |U| = " << input_.data.size() << endl;

WeightedLocalSearchAlgo wls;
ClusteringOutput output1;
wls.perform(input_, output1);
cerr << "INFO: |centers| = " << output1.centers.size() << endl;
}
}
;

int main() { return lest::run(specification); }
