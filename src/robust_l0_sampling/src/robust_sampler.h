#ifndef __ROBUST_SAMPLER_H__
#define __ROBUST_SAMPLER_H__

#include "hash.h"
#include "point.h"

#include <set>
#include <random>
#include <string>
#include <sstream>
#include <map>

class RobustBase {
 public:
  RobustBase(int dimension, double alpha);
  virtual void process(const Point& p);
  // Point get_sample() const;
  virtual std::string to_string() const {
    std::stringstream ss;
    ss << "|Sa| = " << Sa_.size() << std::endl
       << "|Sr| = " << Sr_.size() << std::endl
       << "ghash: " << ghash_.to_str() << std::endl
       << "alpha = " << alpha_ << " side_len = " << side_len_
       << " thresh = " << thresh_;
    return ss.str();
  }

  // returns dimension of the point
  int get_dim() const {
    return offset_.size();
  }
  
  // estimated space used
  // at the moment that this function
  // is called
  virtual int est_space() const;

  // return hash value of cell(p), using phash_
  int cell_hash(const Point& p) const;

 protected:
  Point offset_;
  double thresh_; // the threshold to control when R should be doubled
  GridHash ghash_;
  PointHash phash_;
  std::set<Point> Sa_; // accept set
  std::set<Point> Sr_; // reject set
  double alpha_;  // group diameter
  double side_len_; // side length of the grid

  
  // call this after the ghash is updated.
  // this function will re-sample Sa and Sr
  virtual void update_Sa_Sr();

  // test if a cell is sampled
  bool is_sampled(const Point& cl) const;
  // test if at least one cell in cells is sampled
  bool is_sampled(const std::set<Point>& cells) const;
  // if not first point, return true
  bool not_fst_point(const Point& p) const;
};


class RobustSampler: public RobustBase {
 public:
  RobustSampler(int dimension, double alpha):
      RobustBase(dimension, alpha) {}
  Point get_sample() const;
};


class RobustSampler2: public RobustBase {
 public:
  RobustSampler2(int dimension, double alpha):
      n_pts_after_sample_(0), RobustBase(dimension, alpha) {}
  void process(const Point& p) override;
  Point get_sample() const;
  // check if group(p) is sampled
  bool is_group_sampled(const Point& p);
  int est_space() const override;
  // return number of new points being processed
  // after the latest sample
  int get_n_pts_new() const {
    return n_pts_after_sample_;
  }
 private:
  std::map<int, Point> A_;
  // number of points being processed after
  // the latest sample
  int n_pts_after_sample_;
  void update_Sa_Sr() override;
};



class RobustCounter: public RobustBase {
 public:
  RobustCounter(int dimension, double alpha, double eps):
      RobustBase(dimension, alpha) {
    if (eps <= 0) {
      throw "RobustCount: eps should be larger than 0";
    }
    thresh_ = 5. / eps / eps;
  }
  // estimate f0
  int est_f0() const;
};

#endif


















