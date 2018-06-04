#include "../algorithms/algo_util.h"
#include "../algorithms/centeralizedMultiRoundSampling.h"
#include "../algorithms/kmean_minus_minus.h"
#include "../algorithms/util.h"
#include "../experiments/dataManager.h"

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../experiments/erfanUtil.h"
#include "../lest.hpp"

#include <iostream>
using namespace std;

const lest::test specification[] = {
    CASE("cenmultiroundsampling"){ClusteringInput input;
ClusteringOutput output;
DataSet dataSet;
dataSet.name = "16points";
dataSet.load();
input.n_centers = 4;
input.n_outliers = 0;
input.data = dataSet.data;
CenteralizedMultiRoundSamplingClustering cmrsc(input.n_centers, true, 1);
cmrsc.perform(input, output);

cerr << "----- centers ----" << endl;
for (auto a : output.centers) {
  a.print(" ");
}
cerr << "---- outliers ----" << endl;
for (int i : output.outliers_indices) {
  input.data[i].print(" ");
}

EXPECT(output.centers.size() == input.n_centers);
EXPECT(output.outliers_indices.size() == input.n_outliers);
}
,
}
;

int main() { return lest::run(specification); }
