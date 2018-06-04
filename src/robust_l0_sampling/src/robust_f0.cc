#include "robust_f0.h"
#include "utils.h"
#include "point.h"

#include <map>

void RobustF0::process(const Point& p) {
  auto pp = p + offset_;
  auto cl = to_cell(pp, side_len_);
  f0_.process(cl);
  if ( is_sampled(cl) ) {
    S_[cl] = pp;
  }

  if ( is_sampled( adj(pp, side_len_, alpha_) ) ) {
    bool is_new = true;
    for (const auto& x : Gamma_) {
      if (to_cell(x, side_len_) == cl) {
        is_new = false;
        break;
      }
    }
    if (is_new) {
      Gamma_.insert(pp);
    }
  }
  
  if (f0_.est_f0() > thresh_ * ghash_.get_R()) {
    ghash_.double_r();
    update_S_Gamma();
  }
}

void RobustF0::update_S_Gamma() {
  auto tmp = S_;
  S_.clear();
  for (const auto& pr : tmp) {
    if (is_sampled( pr.first )) {
      S_[pr.first] = pr.second;
    }
  }
  auto bak = Gamma_;
  Gamma_.clear();
  for (const auto& p : bak) {
    if (is_sampled( adj(p, side_len_, alpha_) )) {
      Gamma_.insert(p);
    }
  }
}

int RobustF0::est_space() const {
  int s = ( S_.size() + Gamma_.size() ) * get_dim();
  return RobustBase::est_space() + s + f0_.est_space();
}

int RobustF0::est_f0() const {
  std::map<Point, int> ct;
  for (const auto& pr : S_) {
    ct[pr.first] = 0;
  }
  for (const auto& p : Gamma_) {
    for (const auto& pr : S_) {
      if (dist(pr.second, p) <= alpha_) {
        ct[pr.first]++;
      }
    }
  }

  double w = 0.;
  for (const auto& pr : ct) {
    if (pr.second <= 0) {
      throw "RobustF0: ct == 0";
    }
    w += (1. / pr.second);
  }
  return w * f0_.est_f0() / S_.size();
  // return w * ghash_.get_R();
}
