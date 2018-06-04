#include "../algorithms/simple_algo.h"
#include "../algorithms/algo_util.h"
#include "../experiments/dataManager.h"

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../lest.hpp"

const lest::test specification[] = {
    CASE("RandomClustering"){ClusteringInput input;
ClusteringOutput output;
input.n_centers = 10;
for (int i = 0; i < 100; ++i) {
  arma::vec v(5);
  v.fill(i);
  input.data.push_back(v);
}
input.n_outliers = 5;

RandomClustering algo;
algo.perform(input, output);

EXPECT(output.centers.size() == input.n_centers);
EXPECT(output.outliers_indices.size() == input.n_outliers);
}
,
    /*
    CASE("ArmaKmeansClustering") {
      ClusteringInput input;
      ClusteringOutput output;
      input.n_centers = 10;
      for (int i = 0; i < 100; ++i) {
        arma::vec v(5);
        v.fill(i);
        input.data.push_back(v);
      }
      input.n_outliers = 5;

      ArmaKmeansClustering algo;
      algo.n_iter=10;
      algo.perform(input, output);

      EXPECT( output.centers.size() == input.n_centers );
      EXPECT( output.outliers_indices.size() == input.n_outliers );
    },
    */
    CASE("ArmaKmeansClustering") {
  ClusteringInput input;
  ClusteringOutput output;
  DataSet dataSet;
  dataSet.name = "16points";
  dataSet.load();
  input.n_centers = 4;
  input.n_outliers = 0;
  input.data = dataSet.data;
  ArmaKmeansClustering algo;
  algo.n_iter = 10;
  algo.perform(input, output);

  EXPECT(output.centers.size() == input.n_centers);
  EXPECT(output.outliers_indices.size() == input.n_outliers);
}
}
;

int main() { return lest::run(specification); }
