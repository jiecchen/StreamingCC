#ifndef __UTILS_HPP__
#define __UTILS_HPP__
#include <random>
#include <iostream>


namespace utils {
  const int MAX = 1 << 30;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dis(0, 1);
  std::default_random_engine generator;
  std::uniform_int_distribution<> dis_int(0, MAX);
}



//! Easy-to-use Gaussian Distribution
class Gaussian {
private:
  std::normal_distribution<double> *distPtr;
public:
  //! Constructor
  Gaussian(double m, double sig) {
    distPtr = new std::normal_distribution<double>(m, sig);
  };
  //! return a random number ~ N(m, sig)
  double rand() {
    return (*distPtr)(utils::gen);
  };
  ~Gaussian() {delete distPtr; }
};

//! return random double between 0. and 1.
double rand_double() {
  return utils::dis(utils::gen);
}

//! randomly return from 0, 1, 2, ..., M-1
double rand_int(int M) {
  return utils::dis_int(utils::gen) % M;
}

double find_closest(double a, double b) {
  int j = std::ceil(std::log(b) / std::log(a));
  return std::pow(a, j);
}


#endif
