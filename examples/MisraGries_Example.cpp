/**
   @file MisraGries_Example.cpp
   @author Jiecao Chen (chenjiecao@gmail.com)
   
   This file gives a example of tracking heavy hitters
   in data stream using Misra-Gries algorithm.
*/
#include "../include/streamcc"
#include <iostream>
#include <random>

std::mt19937 gen;


int main() {
  int k = 10;
  Scc::MisraGries<int> mg(k);
  
  // create distribution
  std::discrete_distribution<int> dist {1000, 1, 1, 1, 1, 500, 500, 100, 100, 50, 50, \
      1, 1, 1, 1, 1, 1, 100, 1, 1, 100, 1, 1, 1, 1, 1, 1, 1000};
  // to keep actually frequencies
  std::map<int, int> real;
  
  // process a stream with length 1,000,000
  for (int i = 0; i < 100000; i++) {
    auto p = dist(gen);
    mg.processItem(p);
    real[p] += 1;
  }
  
  // output the top-k
  std::cout << "Top-" << k << " most frequent items:" << std::endl;
  for (auto p : mg.getTopK()) {
    std::cout << "Item: " << p.first << "  Est_Freq: " << p.second << " Actual_Freq: " << real[p.first] << std::endl;
  }
  return 0;
}











