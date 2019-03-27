#include "../src/streamingcc_include/hyper_loglog.h"
#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <random>
#include <map>
#include <iostream>
#include <vector>
#include <set>

using namespace streamingcc;
using namespace std;

BOOST_AUTO_TEST_CASE(util_Test) {
    hyper_loglog count;
    set<int> s;
    for(int i = 0; i < 100000; ++i)
    {
      int cur = rand() % 10000000;
      s.insert(cur);
      count.update(cur);
    }
    int e = count.estimate(), ans = s.size();
    BOOST_CHECK(e >= 0.1 * ans && e <= 10 * ans);
}



















