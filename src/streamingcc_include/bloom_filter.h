// Copyright 2019 Haoyu Zhang

#ifndef SRC_STREAMINGCC_INCLUDE_BLOOM_FILTER_H_
#define SRC_STREAMINGCC_INCLUDE_BLOOM_FILTER_H_

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

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::hash;
using std::to_string;


namespace streamingcc {

class bloom_filter {
 public:
    bloom_filter(): bloom_filter(10000) {}
    explicit bloom_filter(int c);
    void add(int x);
    void add(double x);
    void add(string str);
    bool has(int x);
    bool has(double x);
    bool has(string str);

 private:
    int size;
    vector<int> vec;
    hash<string> hash_fn;
};
}  // namespace streamingcc
#endif  // SRC_STREAMINGCC_INCLUDE_BLOOM_FILTER_H_
