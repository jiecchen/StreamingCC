#include "../src/point.h"
#include "../src/exp_utils.h"

#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <set>

BOOST_AUTO_TEST_CASE(parse_Test) {
  auto s = "1 2 3.0";
  BOOST_CHECK(parse(s) == Point({1, 2, 3.0}));
}

BOOST_AUTO_TEST_CASE(read_data_Test) {
  auto&& dataset = read_data("test_data.txt");
  std::cerr << "#points = " << dataset.size() << std::endl;
  for (const auto& p : dataset) {
    std::cerr << to_str(p) << std::endl;
  }
}











