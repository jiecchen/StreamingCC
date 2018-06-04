#ifndef __UTILS_H__
#define __UTILS_H__

#include "point.h"

#include <vector>
#include <set>

// a cell is also a point, with all coordinates be integers
Point to_cell(const Point& p, double side_len);
double dist(const Point& p1, const Point& p2);

// returns adj(p) = {cell | dist(p, cell) < alpha}
std::set<Point> adj(const Point& p, double side_len, double alpha);



#endif









