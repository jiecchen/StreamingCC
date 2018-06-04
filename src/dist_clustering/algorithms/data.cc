#include "data.h"
#include "util.h"

#include <iostream>
#include <vector>

using namespace std;

namespace data {

vector<arma::vec> uniform(int n_centers, int n_noise) {
  vector<arma::vec> res;
  for (int i = 0; i < n_centers; ++i) {
    res.push_back(arma::vec{i + 0.});
    if (n_noise > 0) {
      auto&& noise = util::sample_with_repl(10000, n_noise);
      for (int d : noise) {
        res.push_back(arma::vec{ i + d / 90000. });
      }
    }
  }
  return res;
}

vector<arma::vec> gauss(int n_centers, int n_noise, double sigma, int dim) {
  arma::arma_rng::set_seed_random();
  vector<arma::vec> res;
  for (int i = 0; i < n_centers; ++i) {
    arma::vec v(dim);
    v.randu();
    res.push_back(v);
    for (int j = 0; j < n_noise; ++j) {
      res.push_back(v + sigma * arma::randn(dim));
    }
  }
  return res;
}


  
};
