#ifndef __HASH_H__
#define __HASH_H__

#include "point.h"

#include <string>
#include <vector>

// a large prime number
const long long HASH_MAX = 236887691; 


class GridHash {
 public:
  GridHash(int R = 1);
  void double_r() {
    R_ *= 2;
  }

  void set_r(int r) {
    R_ = r;
  }
  
  void halve_r() {
    R_ /= 2;
  }
  
  int get_R() const {
    return R_;
  }

  // returns the hash value. x must be non-negative
  int operator()(int x) const {
    long long tmp = (((a_ * x + b_) % HASH_MAX) * x + c_) % HASH_MAX;
    return  tmp % R_;
  }

  std::string to_str() const {
    return "R = " + std::to_string(R_) + ", a = "
        + std::to_string(a_) + ", b = " + std::to_string(b_);
  }

  // returns the estimated space used
  // by this instance
  int est_space() const {
    return 3;
  }
  
 private:
  int R_;
  long long a_;
  long long b_;
  long long c_;
};


// map a point in R^d to a non-negative integer
class PointHash {
 public:
  PointHash(int d);
  int operator()(const Point& p) const;

  // returns the estimated space used
  // by this instance
  int est_space() const {
    return hash_params_.size();
  }

 private:
  std::vector<long long> hash_params_;
};


#endif










