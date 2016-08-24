#include "../src/streamingcc_include/util.h"
#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <random>
#include <map>
#include <iostream>
#include <vector>

using namespace streamingcc::util;

BOOST_AUTO_TEST_CASE(util_Test) {
  std::vector<int> vec = {6, 2, 7, 4, 5, 1, 3};
  int median = CalcMedian(vec);
  BOOST_CHECK(median == 4);
}



















