#include "../src/streamingcc_include/f2.h"
#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <random>
#include <map>
#include <iostream>

BOOST_AUTO_TEST_CASE(F2BasicInt_Test) {
  using streamingcc::integer::F2BasicInt;
  F2BasicInt f2_basic_int(10);
  //  BOOST_CHECK_LE(f2_basic_int.GetEstimation(), 10000);
}



















