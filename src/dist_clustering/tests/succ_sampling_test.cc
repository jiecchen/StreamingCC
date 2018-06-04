#include "../algorithms/data.h"
#include "../algorithms/kmean_minus_minus.h"
#include "../algorithms/succ_sampling.h"
#include "../algorithms/algo_util.h"
#include "../algorithms/util.h"
#include "../experiments/dataManager.h"

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../lest.hpp"

#include <iostream>
using namespace std;

// TODO: add test for SuccSampling
const lest::test specification[] = {
  CASE("SuccSampling"){
    ClusteringInput input;
    ClusteringOutput output;
    DataSet dataSet;
    dataSet.name = "kdd200";
    dataSet.load();
    input.n_centers = 4;
    input.n_outliers = 10;
    input.data = dataSet.data;
    SuccSampling algo;
    algo.setAlpha(2.);
    algo.setBeta(0.3);
    algo.perform(input, output);
    
    EXPECT(output.centers.size() >= input.n_centers);
    EXPECT(output.outliers_indices.size() >= input.n_outliers);
    
    // test when n_outliers == 0
    input.n_outliers = 0;
    algo.perform(input, output);
    EXPECT(output.centers.size() >= input.n_centers);
    EXPECT(output.outliers_indices.size() == 0);
  },
  CASE("SuccSampling2") {
    ClusteringInput input;
    ClusteringOutput output;
    DataSet dataSet;
    dataSet.name = "16points";
    dataSet.load();
    input.n_centers = 4;
    input.n_outliers = 0;
    input.data = dataSet.data;
    SuccSampling algo;
    algo.setAlpha(2.);
    algo.setBeta(0.3);
    algo.perform(input, output);
    cerr << output.centers.size() << endl;
    EXPECT(output.centers.size() >= input.n_centers);
    EXPECT(output.outliers_indices.size() >= input.n_outliers);
    
    // test when n_outliers == 0
    //    input.n_outliers = 0;
    //    algo.perform(input, output);
    //    EXPECT( output.centers.size() >= input.n_centers );
    //    EXPECT( output.outliers_indices.size() == 0 );
  },

};

int main() {
  //  return lest::run(specification);
  auto&& dataset = data::uniform(20, 1000);
  auto&& idx = util::add_outliers(dataset, 50, 10000);
  ClusteringInput input;
  ClusteringOutput output;
  input.n_centers = 20;
  input.n_outliers = 20;
  input.data = dataset;
  input.actual_outliers = idx;
  SuccSampling algo;
  algo.setAlpha(1);
  algo.setBeta(0.5);
  algo.setGamma(8);
  algo.perform(input, output);
  cerr << "|dataset| = " << dataset.size() << endl;
  cerr << "|centers| = " << output.centers.size() << endl;
  cerr << "|actuall_outliers| = " << idx.size() << endl;
  cerr << "|outliers| = " << output.outliers_indices.size() << endl;
  cerr << "jaccard = " << util::jaccard(idx, output.outliers_indices) << endl;
  cerr << "recall = " << util::recall(idx, output.outliers_indices) << endl;
  
  return 0;
}









