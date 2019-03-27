// Copyright 2019 Haoyu Zhang

#ifndef SRC_STREAMINGCC_INCLUDE_HYPER_LOGLOG_H_
#define SRC_STREAMINGCC_INCLUDE_HYPER_LOGLOG_H_

#include <math.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <tuple>
#include <numeric>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <map>
#include <climits>

using std::max;
using std::vector;

namespace streamingcc {

class hyper_loglog {
 public:
    hyper_loglog();
    void update(uint32_t value);
    double estimate();

 private:
    vector<int> vec;
    int b;
    double alpha;
    int firstone(uint32_t n);
    double raw_estimate();
};
}  // namespace streamingcc
#endif  // SRC_STREAMINGCC_INCLUDE_HYPER_LOGLOG_H_
