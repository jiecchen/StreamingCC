#include "../include/CountMin.h"
#include "../include/Hash.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>




CountMin::CountMin(int _m, int _d):
  m(_m), d(_d) {
  srand(time(NULL));
  for (int i = 0; i < d; ++i) {
    d_buf.push_back(Buffer(m));
    seeds.push_back(rand());
  }
}



void CountMin::processItem(const ItemType &item, double weight) {
  for (int i = 0; i < d; ++i) {
    int p = murmurhash(&item, seeds[i]) % m;
    d_buf[i][p] += weight;
  }
}

double CountMin::estTotWeight(const ItemType &item) {
  double values[d];
  for (int i = 0; i < d; ++i) {
    int p = murmurhash(&item, seeds[i]) % m;
    values[i] = d_buf[i][p];
  }
  return *std::min_element(values, values + d);
}

















