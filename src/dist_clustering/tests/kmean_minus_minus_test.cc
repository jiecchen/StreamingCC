#include "../algorithms/kmean_minus_minus.h"
#include "../algorithms/algo_util.h"
#include "../algorithms/util.h"
#include "../experiments/dataManager.h"
#include "../experiments/erfanUtil.h"

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../lest.hpp"

#include <iostream>
using namespace std;

const lest::test specification[] = {
    /*
 CASE("KMeanMinusMinus") {
    ClusteringInput input;
    ClusteringOutput output;
    DataSet dataSet;
    dataSet.name = "1d_points";
    dataSet.load();
    input.n_centers = 3;
    input.n_outliers = 2;
    input.data = dataSet.data;
    KMeanMinusMinus algo;
    algo.perform(input, output);

    cerr << "----- centers ----" << endl;
    for (auto a : output.centers) {
      a.print(" ");
    }
    cerr << "---- outliers ----" << endl;
    for (int i : output.outliers_indices) {
      input.data[i].print(" ");
    }

    EXPECT( output.centers.size() == input.n_centers );
    EXPECT( output.outliers_indices.size() == input.n_outliers );
  },

  CASE("WeightedKMeanMM") {
    ClusteringInput input;
    ClusteringOutput output;
    DataSet dataSet;
    dataSet.name = "1d_points";
    dataSet.load();
    input.n_centers = 3;
    input.n_outliers = 2;
    input.data = dataSet.data;
    vector<double> weights;
    for (int i = 0; i < input.data.size(); ++i) {
      weights.push_back(2);
    }
    input.weights = arma::vec(weights);

    WeightedKMeanMM algo;
    algo.perform(input, output);
    cerr << "----- centers ----" << endl;
    for (auto a : output.centers) {
      a.print(" ");
    }
    EXPECT( output.centers.size() == input.n_centers );
  },

  CASE("expand_points") {
    vector<int> U{1, 2};
    vector<double> weights{1, 2};
    arma::vec w = arma::vec(weights);
    vector<int> exp{1, 2, 1, 2, 2};
    KMMM::expand_points(U, w);
    EXPECT( U == exp );
  },

     */
    CASE("WeightedKMeanMM vs ErfanMM"){ClusteringInput input;
ClusteringOutput outputW, outputE;
DataSet dataSet;
dataSet.name = "spambase";
dataSet.load();

input.n_centers = 2;
input.n_outliers = 0;
input.data = dataSet.data;
input.data.resize(5);
vector<double> weights;
std::cout << input.data.size() << std::endl;
for (int i = 0; i < input.data.size(); ++i) {
  weights.push_back(1);
}
input.weights = arma::vec(weights);

WeightedKMeanMM algoW;
WeightedKMeanMM algoE;
std::cout << "W:" << std::endl;
algoW.perform(input, outputW);
std::cout << "E:" << std::endl;
algoE.perform(input, outputE);
if (false) {
  cerr << "----- centers W ----" << endl;
  for (auto a : outputW.centers) {
    a.print(" ");
  }
  cerr << "----- centers E ----" << endl;
  for (auto a : outputE.centers) {
    a.print(" ");
  }
}
std::vector<int> res_idx;
std::vector<double> min_dist;
util::compute_nearest(outputW.centers, outputE.centers, res_idx, min_dist);
double totalDist = 0;
for (auto& dd : min_dist) {
  totalDist += dd;
}
VARd(totalDist);
outputE.fillExtraInfo(input);
outputW.fillExtraInfo(input);
double xxE =
    cost_outliers(input.data, outputE.centers, outputE.outliers_indices,
                  outputE.labels, util::CostType::k_mean);

double xxW =
    cost_outliers(input.data, outputW.centers, outputW.outliers_indices,
                  outputW.labels, util::CostType::k_mean);

VARd(xxE);
VARd(xxW);
EXPECT(outputW.centers.size() == input.n_centers);
EXPECT(outputE.centers.size() == input.n_centers);
}
/*
CASE("KMM::compute vs util::average")
{
vector<arma::vec> U;
for(int i=0;i<4;i++){
  U.push_back(arma::vec(1,arma::fill::ones));
  U.back().fill(i);
}
vector<int> labels={0,1,2,3};
std::vector<int> O;
int n_centers=4;



vector<arma::vec> Js= KMMM::compute_centers(U,labels,O,n_centers);
vector<arma::vec> Es,oldCenters;
Es.push_back(arma::vec(1,arma::fill::ones));
Es.back().fill(10);
arma::vec weights(U.size(),arma::fill::ones);
weights[3]=0;
util::average_centers(Es,U,weights,labels,O,n_centers,oldCenters);
EXPECT(Es.size()==n_centers);
EXPECT(Js.size()==n_centers);
for(int i=0;i<n_centers;i++){
  Es[i].print("Es[i]");
  Js[i].print("Js[i]");
}


}
 */
}
;

int main() { return lest::run(specification); }
