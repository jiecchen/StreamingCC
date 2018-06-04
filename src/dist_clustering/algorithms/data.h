#ifndef _DATA_H_
#define _DATA_H_


#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include <vector>
namespace data {

// one dimension dataset
// for test purpose
// return 0, 1, 2, ..., n_points - 1;
std::vector<arma::vec> uniform(int n_centers, int n_noise=0);


std::vector<arma::vec> gauss(int n_centers, int n_noise, double sigma, int dim);

};
#endif
