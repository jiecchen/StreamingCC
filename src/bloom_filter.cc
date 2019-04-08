// Copyright (C) 2019 by Haoyu Zhang (kedayuge@gmail.com)

#include "streamingcc_include/bloom_filter.h"

namespace streamingcc {

bloom_filter::bloom_filter(int c) {
           size = c;
        vec.resize(size);
        for (auto& x : vec)
            x = 0;
}

void bloom_filter::add(int x) {
    string str = to_string(x);
    add(str);
}
void bloom_filter::add(double x) {
    string str = to_string(x);
    add(str);
}

void bloom_filter::add(string str) {
    ++vec[hash_fn(str) % size];
    ++vec[size - hash_fn(str) % size];
}

bool bloom_filter::has(int x) {
    string str = to_string(x);
    return has(str);
}
    bool bloom_filter::has(double x) {
    string str = to_string(x);
    return has(str);
}


bool bloom_filter::has(string str) {
    if (vec[hash_fn(str) % size] == 0)
        return false;
    if (vec[size - hash_fn(str) % size] == 0)
        return false;
    return true;
}
}  // namespace streamingcc
