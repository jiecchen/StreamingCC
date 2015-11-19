// Copyright (C) 2015 by Jiecao Chen (chenjiecao@gmail.com)

#include "../include/streamcc_headers/CountSketch_basic.h"
#include "../include/streamcc_headers/Hash.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>


SccAux::CountSketch_basic::CountSketch_basic(int _m, int _d) :
        m(_m), d(_d) {
    srand(time(NULL));
    for (int i = 0; i < d; ++i) {
        d_buf.push_back(Buffer(m));
        seeds.push_back(rand());
        sign_seeds.push_back(rand());
    }
}


void SccAux::CountSketch_basic::processItem(const int &item, double weight) {
    for (int i = 0; i < d; ++i) {
        int p = murmurhash(&item, seeds[i]) % m;
        int sign = murmurhash(&item, sign_seeds[i]) % 2;
        d_buf[i][p] += (sign * 2 - 1) * weight;
    }
}

double SccAux::CountSketch_basic::estTotWeight(const int &item) {
    double values[d];
    for (int i = 0; i < d; ++i) {
        int p = murmurhash(&item, seeds[i]) % m;
        int sign = murmurhash(&item, sign_seeds[i]) % 2;
        values[i] = (sign * 2 - 1) * d_buf[i][p];
    }
    // return the median
    std::nth_element(values, values + d / 2, values + d);
    return values[d / 2];
}

















