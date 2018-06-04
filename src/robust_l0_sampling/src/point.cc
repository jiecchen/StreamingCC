#include "point.h"

#include <cassert>
#include <vector>
#include <string>
#include <random>
#include <cmath>

using namespace std;

Point operator* (const Point& p, double c) {
  Point pp(p.size());
  for (int i = 0; i < p.size(); ++i) {
    pp[i] = p[i] * c;
  }
  return pp;
}

Point operator* (int c, const Point& p) {
  return p * c;
}


Point operator+ (const Point& p1, const Point& p2) {
  assert(p1.size() == p2.size());
  Point pp(p1.size());
  for (int i = 0; i < pp.size(); ++i) {
    pp[i] = p1[i] + p2[i];
  }
  return pp;
}

Point operator+ (const Point& p, double c) {
  auto pp = p;
  for (auto& x : pp) {
    x += c;
  }
  return pp;
}

Point operator- (const Point& p1, const Point& p2) {
  return p1 + (-1 * p2);
}

std::string to_str(const Point& p, std::string delim) {
  string s = "";
  for (int i = 0; i < p.size() - 1; ++i) {
    s = s + to_string(p[i]) + delim;
  }
  s += to_string(p.back());
  return s;
}

Point rand_point(int d, double l, double r) {
  if (l >= r || d <= 0) {
    throw "rand_point: wrong input";
  }
  std::random_device rd; 
  std::mt19937 gen(rd()); 
  std::uniform_real_distribution<> dis(l, r);
  Point p(d, 0);
  for (auto& x : p) {
    x = dis(gen);
  }
  return p;
}

Point nrand_point(int d) {
  auto&& p = rand_point(d);
  // calc l2 norm
  double l2 = 0;
  for (auto x : p) {
    l2 += x * x;
  }
  return p * (1. / std::sqrt(l2 + 0.0001));
}
