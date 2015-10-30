/**
 @file utils.hpp
 @author Jiecao Chen <chenjiecao@gmail.com>

 Provide utility functions, all functions are grouped under utils::
*/


#ifndef __UTILS_HPP__
#define __UTILS_HPP__
#include <random>
#include <iostream>


namespace utils {

  //! randomly return from 0, 1, 2, ..., M-1
  int rand_int(int M=1 << 30);
  
  double find_closest(double a, double b);

  //! return random double between 0. and 1.
  double rand_double();



  //! return number of leading zeros of x
  uint32_t zeros(uint32_t x);


  //! Easy-to-use Gaussian Distribution
  class Gaussian {
  private:
    std::normal_distribution<double> dist;
  public:
    //! Constructor
    Gaussian(double m, double sig): dist(m, sig) {};
    //! return a random number ~ N(m, sig)
    double rand();
  };
}







#endif
