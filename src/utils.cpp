#include "../include/streamcc_headers/utils.hpp"
#include <random>
#include <cmath>
#include <stdint.h>



int utils::rand_int(int M) {
  return dis_int(gen) % M;
}


double utils::find_closest(double a, double b) {
  int j = std::ceil(std::log(b) / std::log(a));
  return std::pow(a, j);      
}


double utils::rand_double() {
    return dis(gen);
}


double utils::Gaussian::rand() {
  return dist(gen);
}

//! adapted from [blog](http://embeddedgurus.com/state-space/2014/09/fast-deterministic-and-portable-counting-leading-zeros/)
uint32_t utils::zeros(uint32_t x) {
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












