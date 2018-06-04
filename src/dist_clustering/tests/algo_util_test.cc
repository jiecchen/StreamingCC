#include "../algorithms/algo_util.h"
#include "../algorithms/simple_algo.h"
#include "../experiments/dataManager.h"

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../lest.hpp"

const lest::test specification[] = {
    CASE("ClusteringInput "){ClusteringInput input;
input.n_outliers = 1;
EXPECT(input.n_outliers == 1);
}
, CASE("ClusteringAlgo") {
  RandomClustering rc;
  //      std::cout<<rc.getName()<<std::endl;
  //      EXPECT( input.n_outliers == 1 );
}
,

    CASE("compute_weights") {
  ClusteringInput input;
  ClusteringOutput output;
  ArmaKmeansClustering algo;
  algo.n_iter = 10;

  DataSet dataSet;
  dataSet.name = "16points";
  dataSet.folderName = "../data/";
  dataSet.load();
  input.data = dataSet.data;
  input.n_centers = 3;
  input.n_outliers = 4;
  // a perfect solver gives this output
  arma::vec v1 = {2, 2};
  output.centers.push_back(v1);
  arma::vec v2 = {102, 2};
  output.centers.push_back(v2);
  arma::vec v3 = {2, 102};
  output.centers.push_back(v3);

  std::vector<int> ov = {12, 13, 14, 15};
  output.outliers_indices = ov;
  output.labels = {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};

  arma::vec testWeights = {4, 4, 4};
  output.make_sure_labels_weights_outliers(input);
  //      output.weights.print("output.weights");
  //      std::cout<<output.labels.size()<<std::endl;
  //      std::cout<<output.outliers_indices.size()<<std::endl;
  EXPECT(approx_equal(testWeights, output.weights, "abs", 0.1));
}
}
;

int main() { return lest::run(specification); }
