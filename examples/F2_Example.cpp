/**
   @file F2_Example.cpp
   @author Jiecao Chen <chenjiecao@gmail.com>
   
   This file gives a example of estimating 2rd
   moment of the frequency vector of a weighted
   data stream using class `F2`
*/

#include "../include/streamcc"
#include <iostream>
#include <vector>
#include <map>
int main() {

  std::vector<char> stream = {'a', 'a', 'a', 'a', 'a', 'a', 'a', 'b', 'b', 'c', 'c', 'd', 'e'};
  // create an instance of F2 with buffer size 20
  Scc::F2<char> f2(20);

  // process items in the data stream
  for (auto itm: stream) {
    f2.processItem(itm, 2);
  }

  // output result
  std::cout << "Estimated F2 = " << f2.getEstF2() << std::endl;
  return 0;
}





