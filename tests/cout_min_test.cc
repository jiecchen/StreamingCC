#include "../src/streamingcc_include/count_min.h"
#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <random>
#include <map>
#include <iostream>


BOOST_AUTO_TEST_CASE(CountMinInt_Test) {
  using streamingcc::integer::CountMinInt;
  CountMinInt count_min(20, 20);
  for (int i = 0; i < 30; ++i) {
    for (int j = 0; j < i * i * i; ++j) {
      count_min.ProcessItem(i, 1.); 
    }
  }
  for (int i = 25; i < 30; ++i) {
    auto est = count_min.GetEstimation(i);
    BOOST_CHECK_MESSAGE(est < i * i * i * 1.2  && est > i * i * i *  0.8,
                        "CountMinInt::GetEstimation = " << est);  
  }
}










