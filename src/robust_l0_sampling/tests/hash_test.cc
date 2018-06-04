#include "../src/hash.h"
#include "../src/point.h"

#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <map>
#include <algorithm>
#include <string>


BOOST_AUTO_TEST_CASE(GridHash_Test) {
  GridHash gh;
  gh.double_r();
  gh.double_r();
  gh.double_r();
  gh.double_r();
  std::map<int, int> ct;
  for (int i = 0; i < gh.get_R(); ++i) {
    ct[i] = 0;
  }
  for (int i = 0; i < 10000; i++) {
    int d = gh(i * i);
    ct[d]++;
  }
  std::vector<int> cc;
  for (auto p : ct) {
    cc.push_back(p.second);
  }

  auto mi = *std::min_element(cc.begin(), cc.end());
  auto ma = *std::max_element(cc.begin(), cc.end());
  BOOST_CHECK_MESSAGE(1.4 * mi > ma, std::to_string(mi) + " " << std::to_string(ma) );
}


