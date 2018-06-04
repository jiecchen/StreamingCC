#include "f0.h"

#include <random>
#include <iostream>
#include <algorithm>


F0_Basic::F0_Basic(int dimension, double eps):
    phash_(dimension), thresh_(5. / eps / eps) {
  if (eps <= 0 || dimension <= 0) {
    throw "F0_Basic: esp <= 0 or dimension <=0";
  }
}

void F0_Basic::process(const Point& p) {
  int h = phash_(p);
  if (ghash_(h) == 0) {
    S_.insert(h);
  }
  if (S_.size() > thresh_) {
    ghash_.double_r();
    update_S();
  }
}


void F0_Basic::update_S() {
  auto tmp = S_;
  S_.clear();
  for (auto h : tmp) {
    if (ghash_(h) == 0) {
      S_.insert(h);
    }
  }
}

int F0_Basic::est_f0() const {
  //  std::cerr << "R = " << R_ << ", |S| = " << S_.size() << std::endl;
  return S_.size() * ghash_.get_R();
}

int F0_Basic::est_space() const {
  return ghash_.est_space() + phash_.est_space()
      + S_.size() + 1;
}






F0::F0(int dimension, double eps, int mu) {
  if (mu < 2 || dimension < 1 || eps <= 0 || eps > 1) {
    throw "F0: wrong arguments";
  }
  for (int i = 0; i < mu; ++i) {
    f0bs_.emplace_back(dimension, eps);
  }
}

void F0::process(const Point& p) {
  for (auto& fb : f0bs_) {
    fb.process(p);
  }
}

int F0::est_space() const {
  int s = 0;
  for (const auto& fb : f0bs_) {
    s += fb.est_space();
  }
  return s;
}

int F0::est_f0() const {
  std::vector<int> v;
  for (const auto& fb : f0bs_) {
    v.push_back(fb.est_f0());
  }
  std::nth_element(v.begin(), v.begin() + v.size()/2, v.end());
  return v[v.size() / 2];
}









