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

  //! randomly return from 0, 1, 2, ..., M-1
  double rand_int(int M) {
    return utils::dis_int(utils::gen) % M;
  }
  
  double find_closest(double a, double b) {
    int j = std::ceil(std::log(b) / std::log(a));
    return std::pow(a, j);    
  }

  //! return random double between 0. and 1.
  double rand_double() {
    return utils::dis(utils::gen);
  }



  //! return number of tailing zeros
  static inline uint32_t zeros(uint32_t x) {
    static uint8_t const clz_lkup[] = {
      32U, 31U, 30U, 30U, 29U, 29U, 29U, 29U,
      28U, 28U, 28U, 28U, 28U, 28U, 28U, 28U
    };
    uint32_t n;

    if (x >= (1U << 16)) {
      if (x >= (1U << 24)) {
	if (x >= (1 << 28)) {
	  n = 28U;
	}
	else {
	  n = 24U;
	}
      }
      else {
	if (x >= (1U << 20)) {
	  n = 20U;
	}
	else {
	  n = 16U;
	}
      }
    }
    else {
      if (x >= (1U << 8)) {
	if (x >= (1U << 12)) {
	  n = 12U;
	}
	else {
	  n = 8U;
	}
      }
      else {
	if (x >= (1U << 4)) {
	  n = 4U;
	}
	else {
	  n = 0U;
	}
      }
    }
    return (uint32_t)clz_lkup[x >> n] - n;
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

}







#endif
