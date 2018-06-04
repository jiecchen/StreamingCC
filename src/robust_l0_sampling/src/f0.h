#ifndef __F0_H__
#define __F0_H__

#include "point.h"
#include "hash.h"

#include <set>
#include <random>
#include <vector>

class F0_Basic {
 public:
  F0_Basic(int dimension, double eps);
  void process(const Point& p);
  int est_f0() const;
  
  // returns estimated space usage
  // of the moment this function is called
  int est_space() const;
 private:
  GridHash ghash_;
  PointHash phash_;
  std::set<int> S_;
  double thresh_;
  void update_S();
};


class F0 {
 public:
  F0(int dimension, double eps, int mu=30);
  void process(const Point& p);
  int est_f0() const;

  int est_space() const;
 private:
  std::vector<F0_Basic> f0bs_;
};


#endif







