#include "../experiments/dataManager.h"
#include <iostream>
#include "../algorithms/algo_util.h"
#include "../algorithms/simple_algo.h"

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "../lest.hpp"
double eps = 0.001;

const lest::test specification[] = {CASE("16points dataSet"){DataSet dataSet;
dataSet.name = "16points";
dataSet.load();
EXPECT(dataSet.data.size() == 16);
EXPECT(dataSet.data.at(15)(1) == 104);
EXPECT(true);
}
, CASE("spambase dataSet") {
  DataSet dataSet;
  dataSet.name = "spambase";
  dataSet.load();

  EXPECT(dataSet.data.size() == 4601);
  EXPECT(dataSet.data.at(15).n_elem == 58);
  double lastRowSum = arma::sum(dataSet.data.at(4600));
  EXPECT(abs(lastRowSum - 55.545) < eps);
}
, CASE("gausmixture dataSet") {
  DataSet dataSet;
  dataSet.name = "gausmixture";
  dataSet.load();
  EXPECT(dataSet.data.size() == 10000);
  EXPECT(dataSet.data.at(15).n_elem == 15);
  double lastRowSum = arma::sum(dataSet.data.at(9999));
  EXPECT(abs(lastRowSum - 29.4806587268) < eps);
}
, CASE("kdd200 dataSet") {
  DataSet dataSet;
  dataSet.name = "kdd200";
  dataSet.load();

  EXPECT(dataSet.data.size() == 200);
  EXPECT(dataSet.data.at(15).n_elem == 145);
  double lastRowSum = arma::sum(dataSet.data.at(199));
  EXPECT(abs(lastRowSum - 1021.05) < eps);
}
}
;

int main() { return lest::run(specification); }
