#include "../src/point.h"
#include "../src/f0.h"

#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <set>


BOOST_AUTO_TEST_CASE(F0_Basic_Test) {
  F0_Basic f0(1, 0.1);
  for (int i = 0; i < 10000; i++) {
    for (int j = 0; j <= 10; ++j) {
      f0.process(Point({i + .0}));
    }
  }
  auto est = f0.est_f0();
  BOOST_CHECK(est * 1.1 > 10000 & est * 0.9 < 10000);
}


BOOST_AUTO_TEST_CASE(F0_Test) {
  F0 f0(1, 0.1);
  for (int i = 0; i < 10000; i++) {
    for (int j = 0; j <= 10; ++j) {
      f0.process(Point({i + .0}));
    }
  }
  auto est = f0.est_f0();
  BOOST_CHECK(est * 1.1 > 10000 & est * 0.9 < 10000);
}












