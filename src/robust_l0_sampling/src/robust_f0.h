#ifndef __ROBUST_F0_H__
#define __ROBUST_F0_H__

#include "robust_sampler.h"
#include "f0.h"
#include "point.h"

#include <set>
#include <vector>
#include <map>

// using F0 = F0_Basic;

class RobustF0: public RobustBase {
 public:
  RobustF0(int dimension, double alpha, double eps):
  RobustBase(dimension, alpha), f0_(dimension, eps, 30) {
    thresh_ = 5. / eps / eps;
  }
  void process(const Point& p) override;
  int est_f0() const;

  // returns estimated space of the moment
  // that this function is called
  int est_space() const override;
  
  std::string to_string() const override {
    std::stringstream ss;
    ss << "|S| = " << S_.size() << std::endl
       << "|Gamma| = " << Gamma_.size() << std::endl
       << "f0 of sampled = " << f0_.est_f0() << std::endl
       << "ghash: " << ghash_.to_str() << std::endl
       << "alpha = " << alpha_ << " side_len = " << side_len_
       << " thresh = " << thresh_;
    return ss.str();
  }

  
 private:
  // call this after R is doubled
  // this function re-samples points in S_ and Gamma_
  void update_S_Gamma();

  // key: non-empty cells that are sampled
  // value: one of the corresponding points
  std::map<Point, Point> S_;
  // points in different cells and whose adj are sampled 
  std::set<Point> Gamma_;
  F0 f0_;
};

#endif


  
