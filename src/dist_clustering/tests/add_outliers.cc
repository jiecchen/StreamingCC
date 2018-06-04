#include "../algorithms/util.h"
#include "../experiments/dataManager.h"


#include <iostream>
#include <cstdlib>
#include <fstream>

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"


using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 5) {
    cerr << "usage: ./add_outliers dataset n_points upper_bound output_file" << endl;
    return 0;
  }
  DataSet ds;
  ds.name = argv[1];
  int n_points = atoi(argv[2]);
  int upper_bound = atoi(argv[3]);
  ds.load();
  util::add_outliers(ds.data, n_points, upper_bound);

  arma::mat data(ds.data.size(), ds.data[0].size());
  util::convert_to_mat(data, ds.data);
  ofstream fout(argv[4], fstream::out);
  data.save(fout, arma::csv_ascii);
  
  return 0;
}
