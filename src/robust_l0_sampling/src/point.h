#ifndef __POINT_H__
#define __POINT_H__
// this file define a point as vector<double>

#include <vector>
#include <string>

using Point = std::vector<double>;


Point operator* (const Point& p, double c);
Point operator* (int c, const Point& p);

Point operator+ (const Point& p1, const Point& p2);
Point operator+ (const Point& p1, double c);
Point operator- (const Point& p1, const Point& p2);

// std::string to_str(const Point& p);
std::string to_str(const Point& p, std::string delim=" ");
// returns a random point in R^d
// each coordinate is drawn from (l, r) uniformly
Point rand_point(int d, double l=0., double r=1.);

// returns a normalized random point in R^d. L2 norm = 1
Point nrand_point(int d);

#endif









