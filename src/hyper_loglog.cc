// Copyright (C) 2019 by Haoyu Zhang (kedayuge@gmail.com)

#include "streamingcc_include/hyper_loglog.h"
#include "streamingcc_include/hash.h"

namespace streamingcc {

hyper_loglog::hyper_loglog() {
    b = 5;
    alpha = 0.697;
    vec.resize(32);
    for (auto& x : vec) {
        x = 0;
    }
}

double hyper_loglog::raw_estimate() {
    double res = 0;
    for (auto x : vec) {
        res += pow(2, -x);
    }
    res = 1.0 / res;
    res *= 32 * 32 * alpha;
    return res;
}

double hyper_loglog::estimate() {
    double res = raw_estimate();
    if (res < 80) {
        int c = std::count(vec.begin(), vec.end(), 0);
        return c == 0 ? res: 32 * log2(32 / c);
    } else if (res > pow(2, 32) / 30) {
        return -pow(2, 32) * log2(1 - res/pow(2, 32));
    }
    return res;
}


void hyper_loglog::update(uint32_t value) {
    uint32_t x = murmurhash(value, 17);
    int j = (x >> (32 - b));
    int w = firstone(x);
    vec[j] = max(vec[j], w);
}

int hyper_loglog::firstone(uint32_t n) {
    for (int i = 0; i < 26; ++i) {
        if (n % static_cast<int>(pow(2, i))) {
            return i;
        }
    }
    return 26;
}
}  // namespace streamingcc
