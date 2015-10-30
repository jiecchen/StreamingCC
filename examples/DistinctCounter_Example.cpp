/**
   @file DistinctCounter_Example.cpp
   @author Jiecao Chen (chenjiecao@gmail.com)
   
   This file gives a example of estimating #
   of distinct element of a data stream using
   class `DistinctCounter`
*/
#include "../include/streamcc"
#include <iostream>

int main() {
  // create a instance with buffer size 20
  Scc::DistinctCounter<int> f0(20);

  // there are 10000 distinct elements
  // each repeat 10 times
  for (int i = 0; i < 100000; ++i) {
    for (int j = 0; j < 10; ++j)
      f0.processItem(i);
  }
  
  // output the estimation
  std::cout << "# of distinct elements = " << f0.getEstDistinct() << std::endl;
  return 0;
}
