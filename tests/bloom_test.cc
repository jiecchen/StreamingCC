#include "../src/streamingcc_include/bloom_filter.h"
#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <random>
#include <map>
#include <iostream>
#include <vector>

using namespace streamingcc;

BOOST_AUTO_TEST_CASE(util_Test) {
    bloom_filter filter(100);
    filter.add("ac");
    filter.add("bd");
    filter.add("ck");
    filter.add("ddd");
    filter.add("axxd");
    filter.add(11);
    filter.add(2.321);
    filter.add("sfsdgdsg");
    bool check = filter.has("ac");
    BOOST_CHECK(check == true);
    check = filter.has(2.321);
    BOOST_CHECK(check == true);
    check = filter.has(11);
    BOOST_CHECK(check == true);

    check = filter.has(12);
    BOOST_CHECK(check == false);
    check = filter.has("ab");
    BOOST_CHECK(check == false);

}



















