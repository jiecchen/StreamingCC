#include "../src/point.h"
#include "../src/utils.h"

#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <set>


BOOST_AUTO_TEST_CASE(Adj_Test) {
  double side_len = 2;
  double alpha = 1;
  Point p{0.5};
  std::set<Point> s{{-1}, {0}};
  BOOST_CHECK(adj(p, side_len, alpha) == s);
}


BOOST_AUTO_TEST_CASE(to_cell_Test) {
  Point p{5, 2, 1};
  double side_len = 2;
  Point pp{2, 1, 0};
  BOOST_CHECK(to_cell(p, side_len) == pp);
}


BOOST_AUTO_TEST_CASE(dist_Test) {
  Point p{2, 1, 0};
  Point pp{5, 1, 4};
  BOOST_CHECK(dist(p, pp) == 5);
}











