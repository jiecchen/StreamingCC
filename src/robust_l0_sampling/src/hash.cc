#include "hash.h"

#include <random>
#include <cassert>
#include <cmath>

GridHash::GridHash(int R): R_(R) {
  std::random_device rd; 
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<> dis(0, HASH_MAX);
  a_ = dis(gen);
  b_ = dis(gen);
  c_ = dis(gen);
}


PointHash::PointHash(int d) {
  std::random_device rd; 
  std::mt19937 gen(rd()); 
  std::uniform_int_distribution<> dis(0, HASH_MAX);
  for (int i = 0; i <= d; ++i) {
    hash_params_.push_back(dis(gen));
  }
}


int PointHash::operator()(const Point& p) const {
  size_t d = hash_params_.size() - 1;
  if (p.size() != d) {
    throw "PointHash(): dimension error";
  }
  long long h = hash_params_[0];
  int i = 1;
  for (auto x : p) {
    long long xx = std::floor(x);
    h = (h + xx * hash_params_[i++]) % HASH_MAX;
  }
  return h;
}








