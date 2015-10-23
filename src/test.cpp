#include <vector>
#include <iostream>
#include "../include/Hash.h"
#include "../include/CountMin.hpp"
#include "../include/Sampling.hpp"
#include <random>
#include <map>

std::mt19937 gen;

template <typename T>
void print_vector(std::vector<T> vec) {
  for (auto v: vec)
    std::cout << v << std::endl;
}

void test_CountMin() {
  CountMin<char> cm(20, 5);
  std::discrete_distribution<int> dist {100, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::map<int, int> real;

  for (int i = 0; i < 100000; ++i) {
    int p = dist(gen);
    cm.processItem(p);
    real[p] += 1;
  }
  
  for (int i = 0; i < 15 ; ++i) {
    std::cout << i << ": " << cm.estTotWeight(i) << ":" << real[i] << std::endl;
  }
}

void test_CountSketch() {
  CountMin_basic cs(10, 5);
  std::discrete_distribution<int> dist {100, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  std::map<int, int> real;

  for (int i = 0; i < 100000; ++i) {
    int p = dist(gen);
    cs.processItem(p);
    real[p] += 1;
  }
  
  for (int i = 0; i < 15 ; ++i) {
    std::cout << i << ": " << cs.estTotWeight(i) << ":" << real[i] << std::endl;
  }

} 


void test_Sampling() {
  // std::discrete_distribution<int> dist {100, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  UniformSampling<int> usp(10, true);
  for (int i = 0; i < 100; i++)
    usp.processItem(i);
  print_vector(usp.getSamples());
}


int main() {
  // test_CountMin();
  // std::cout << "----------------------------------" << std::endl;
  //  test_CountSketch();
  test_Sampling();
  return 0;
}
















