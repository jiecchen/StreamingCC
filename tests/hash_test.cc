#include "../src/streamingcc_include/hash.h"
#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <random>
#include <map>
#include <iostream>
#include <vector>
#include <cstdint>

inline int Sign(uint32_t num) {
  return (num % 2) * 2 - 1;
}

BOOST_AUTO_TEST_CASE(murmurhash_Test) {
  using streamingcc::murmurhash;
  BOOST_CHECK(murmurhash(10, 20) == murmurhash(10, 20));
  BOOST_CHECK(murmurhash(10, 201) == murmurhash(10, 201));
  BOOST_CHECK(murmurhash(11, 20) != murmurhash(10, 20));
  BOOST_CHECK(murmurhash(10, 21) != murmurhash(10, 20));
  // test if roughly give 50% +-1
  const int kN = 1000;
  const uint32_t kSeed = 1;
  int count_neg = 0;
  for (uint32_t i = 0; i < kN; ++i) {
    if (Sign(murmurhash(i, kSeed)) < 0) ++count_neg;
  }
  BOOST_CHECK_MESSAGE(count_neg * 2 < kN + 70 && count_neg * 2 > kN - 70,
                      "negative/total = " << count_neg << "/" << kN);
}



















