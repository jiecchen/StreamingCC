#include "../include/streamcc"
#include <iostream>
#include <random>


int main() {
  // set up a pseudo-random generator
  std::mt19937 gen;
  // specify a discrete distribution as a data stream generator
  std::discrete_distribution<int> dist {100, 5, 5, 1, 1,\
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

  // create an instance of count-sketch
  // with buffer size as 20 
  Scc::CountSketch<int> cs(20);

  // process a data stream with length 1,000,000
  for (int i = 0; i < 1000000; ++i) {
    cs.processItem(dist(gen));
  }
  
  // estimate the frequency of item 0 ~ 14
  for (int i = 0; i < 15 ; ++i) {
    std::cout << "Frequency of " << i << " = " 
	      << cs.estTotWeight(i) << std::endl;
  }
  return 0;
}
