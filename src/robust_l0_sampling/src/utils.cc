#include "utils.h"
#include "point.h"

#include <cassert>
#include <cmath>
#include <vector>
#include <set>
#include <iostream>

using namespace std;

Point to_cell(const Point& p, double side_len) {
  assert(side_len > 0);
  Point pp = p;
  for (auto& c :  pp) {
    c = floor(c / side_len);
  }
  return pp;
}


double dist(const Point& p1, const Point& p2) {
  if (p1.size() != p2.size()) {
    throw "dist: dimension not aligned!";
  }
  auto pp = p1 - p2;
  double s = 0.;
  for (auto x: pp) {
    s += x * x;
  }
  return sqrt(s);
}

// will add adj into cells
void internal_adj(const vector<vector<double>>& delta, int i, double accum,
                  vector<int>& dims, const Point& p, double alpha,
                  double side_len, set<Point>& cells) {
  if ( accum > pow(alpha, 2) ) {
    return;
  }
  if (i >= p.size()) {
    Point pp;
    for (int j = 0; j < p.size(); ++j) {
      pp.push_back(delta[dims[j]][j]);
    }
    double norm_pp = sqrt(accum);
    if (norm_pp > 0) {
      pp = pp * (alpha / norm_pp); 
    }
    cells.insert(to_cell(p - pp, side_len));
    return;
  }

  dims[i] = 0;
  internal_adj(delta, i + 1, accum, dims, p, alpha, side_len, cells);
  dims[i] = 1;
  internal_adj(delta, i + 1, accum + pow(delta[1][i], 2), dims, p, alpha, side_len, cells);
  dims[i] = 2;
  internal_adj(delta, i + 1, accum + pow(delta[2][i], 2), dims, p, alpha, side_len, cells);
}

set<Point> adj(const Point& p, double side_len, double alpha) {
  // get the dimension of point
  int d = p.size();
  // when p is closed to boundary, we need to add small shift
  auto pp = p + 0.001;
  set<Point> cells;
  auto cl = to_cell(pp, side_len);
  
  // 0 - no shift,  1 - down,  2 - up
  vector<vector<double>> delta(3);
  delta[0] = Point(d, 0);
  for (int i = 0; i < d; ++i) {
    delta[1].push_back(pp[i] - cl[i] * side_len);
  }
  for (int i = 0; i < d; ++i) {
    delta[2].push_back(delta[1][i] - side_len);
  }
  // dims to record how to shift in each dimension
  vector<int> dims(d);

  internal_adj(delta, 0, 0, dims, pp, alpha, side_len, cells);
  return cells;
}


















