#include "../algorithms/util.h"
#include "../algorithms/data.h"



#include <iostream>
#include <cstdlib>
#include <fstream>

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"


using namespace std;

int main(int argc, char* argv[]) {
  if (argc != 6) {
    cerr << "usage: ./gen_gauss n_centers n_noise sigma dim output_file" << endl;
    return 0;
  }

  const int n_centers = atoi(argv[1]);
  const int n_noise = atoi(argv[2]);
  const double sigma = atof(argv[3]);
  const int dim = atoi(argv[4]);

  auto&& dataset = data::gauss(n_centers, n_noise, sigma, dim);
  
  arma::mat mat(dataset.size(), dim);
  util::convert_to_mat(mat, dataset);
  ofstream fout(argv[5], fstream::out);
  mat.save(fout, arma::csv_ascii);
  
  return 0;
}
