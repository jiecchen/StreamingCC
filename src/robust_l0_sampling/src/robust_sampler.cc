#include "robust_sampler.h"
#include "utils.h"
#include "point.h"

#include <random>
#include <vector>
#include <set>
#include <iostream>


RobustBase::RobustBase(int dimension, double alpha):
    ghash_(), phash_(dimension), alpha_(alpha) {
  // create a random offset_, as the effect of placing random grid
  std::random_device rd; 
  std::mt19937 gen(rd()); 
  std::uniform_real_distribution<> dis(0, dimension * 10);
  offset_ = Point(dimension, 0);
  for (auto& c : offset_) {
    c = dis(gen);
  }
  //  std::cerr << "offset = " << to_str(offset_) << std::endl;
  

  ghash_ = GridHash();
  phash_ = PointHash(dimension);
  side_len_ = dimension; //!!! to be changed
  thresh_ = 30;
}

int RobustBase::est_space() const {
  return (Sa_.size() + Sr_.size() + 1) * offset_.size()
      + ghash_.est_space() + phash_.est_space(); 
}

int RobustBase::cell_hash(const Point& p) const {
  return phash_( to_cell(p, side_len_) );
}

void RobustBase::process(const Point& p) {
  auto pp = p + offset_;
  if (not_fst_point(pp)) {
    return;
  }

  // insert pp to Sa or Sr if possible
  if ( is_sampled( to_cell(pp, side_len_) ) ) {
    Sa_.insert(pp);
  } else if ( is_sampled( adj(pp, side_len_, alpha_) ) ) {
    Sr_.insert(pp);
  }

  if (Sa_.size()  > thresh_) {
    ghash_.double_r();
    update_Sa_Sr();
  }
}



bool RobustBase::is_sampled(const Point& cl) const {
  int h = phash_(cl);
  return ghash_(h) == 0;
}

bool RobustBase::is_sampled(const std::set<Point>& cells) const {
  for (const auto& cl : cells) {
    if (is_sampled(cl)) {
      return true;
    }
  }
  return false;
}

bool RobustBase::not_fst_point(const Point& p) const {
  for (const auto& pp : Sa_) {
    if (dist(p, pp) <= alpha_) {
      return true;
    }
  }
  for (const auto& pp : Sr_) {
    if (dist(p, pp) <= alpha_) {
      return true;
    }
  }
  return false;
}




void RobustBase::update_Sa_Sr() {
  std::set<Point> tmpa = Sa_;
  std::set<Point> tmpb = Sr_;
  Sr_.clear();
  Sa_.clear();
  
  for (const auto& p : tmpa) {
    if (is_sampled( to_cell(p, side_len_) )) {
      Sa_.insert(p);
    } else if (is_sampled( adj(p, side_len_, alpha_) )) {
      Sr_.insert(p);
    }
  }
  for (const auto& p : tmpb) {
    if (is_sampled( adj(p, side_len_, alpha_) )) {
      Sr_.insert(p);
    }
  }
}




Point RobustSampler::get_sample() const {
  if (Sa_.size() <= 0) {
    throw "get_sample: Sa is empty!";
  }
  std::vector<Point> vec(Sa_.begin(), Sa_.end());
  std::random_device rd; 
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<> dis(0, vec.size() - 1);
  return vec[dis(gen)] - offset_;
}


void RobustSampler2::process(const Point& p) {
  ++n_pts_after_sample_;
  auto pp = p + offset_;

  for (const auto& p1 : Sa_) {
    if (dist(p1, pp) <= alpha_) {
      int h = phash_( to_cell(p1 , side_len_) );
      A_[h] = pp;
      // new point sampled, so reset
      n_pts_after_sample_ = 0;
      return;
    }
  }
  
  if (not_fst_point(pp)) {
    return;
  }

  // insert pp to Sa or Sr if possible
  if ( is_sampled( to_cell(pp, side_len_) ) ) {
    Sa_.insert(pp);
    int h = phash_( to_cell(pp , side_len_) );
    A_[h] = pp;
    // new point sampled, so reset
    n_pts_after_sample_ = 0;
  } else if ( is_sampled( adj(pp, side_len_, alpha_) ) ) {
    Sr_.insert(pp);
    return;
  }

  if (Sa_.size()  > thresh_) {
    ghash_.double_r();
    if (is_sampled( to_cell(pp, side_len_) )) {
      update_Sa_Sr();
    } else {
      ghash_.halve_r();
    }
  }
}


void RobustSampler2::update_Sa_Sr() {
  RobustBase::update_Sa_Sr();
  auto tmp = A_;
  A_.clear();
  for (const auto& p : Sa_) {
    int h = phash_( to_cell(p, side_len_) );
    A_[h] = tmp[h];
  }
}

bool RobustSampler2::is_group_sampled(const Point& p) {
  auto pp = p + offset_;
  for (const auto& p1 : Sa_) {
    if (dist(p1, pp) <= alpha_) {
      return true;
    }
  }
  return false;
}


Point RobustSampler2::get_sample() const {
  if (Sa_.size() <= 0) {
    throw "get_sample: Sa is empty!";
  } 
  std::vector<Point> vec(Sa_.begin(), Sa_.end());
  std::random_device rd; 
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<> dis(0, vec.size() - 1);
  int h = phash_( to_cell(vec[dis(gen)], side_len_) );
  return A_.at(h) - offset_;
}

int RobustCounter::est_f0() const {
  return (int) Sa_.size() * ghash_.get_R();
}

int RobustSampler2::est_space() const {
  return A_.size() * get_dim() + RobustBase::est_space();
}








