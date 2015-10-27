#include "../include/streamcc_test.h"
#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <random>
#include <map>





std::mt19937 gen;


BOOST_AUTO_TEST_CASE(CountSketch_Test) {
  Scc::CountSketch<int> cs(100, 20);
  std::discrete_distribution<int> dist {100, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::map<int, int> real; // to keep the actual frequencies

  for (int i = 0; i < 100000; ++i) {
    int p = dist(gen);
    cs.processItem(p);
    real[p] += 1;
  }
  
  // now check the frequencies
  for (int i = 0; i < 15 ; ++i) {
    BOOST_CHECK(cs.estTotWeight(i) == real[i]);
  }
}


BOOST_AUTO_TEST_CASE(CountMin_Test) {
  Scc::CountMin<int> cm(100, 20);
  std::discrete_distribution<int> dist {100, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::map<int, int> real; // to keep the actual frequencies

  for (int i = 0; i < 100000; ++i) {
    int p = dist(gen);
    cm.processItem(p);
    real[p] += 1;
  }
  
  // now check the frequencies
  for (int i = 0; i < 15 ; ++i) {
    BOOST_CHECK(cm.estTotWeight(i) == real[i]);
  }
}























