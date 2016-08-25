#include "../src/streamingcc_include/f2.h"
#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <random>
#include <map>
#include <iostream>


BOOST_AUTO_TEST_CASE(F2BasicInt_Test) {
  using streamingcc::integer::F2BasicInt;
  F2BasicInt f2b(20);
  for (int i = 0; i < 100; ++i) {
    for (int j = 0; j < 10; ++j) {
      f2b.ProcessItem(i, 1.); 
    }
  }
  auto f2_est = f2b.GetEstimation();
  BOOST_CHECK_MESSAGE(f2_est < 20000,
                      "f2::GetEstimation = " << f2_est);  
}

BOOST_AUTO_TEST_CASE(F2Int_Test) {
  using streamingcc::integer::F2Int;
  F2Int f2(50, 20);
  for (int i = 0; i < 100; ++i) {
    for (int j = 0; j < 10; ++j) {
      f2.ProcessItem(i); 
    }
  }
  auto f2_est = f2.GetEstimation();
  BOOST_CHECK_MESSAGE(f2_est < 10000 + 2000 && f2_est > 10000 - 2000,
                      "f2::GetEstimation = " << f2_est);
}
