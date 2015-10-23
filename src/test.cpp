#include <vector>
#include <iostream>
#include "../include/Hash.h"
#include "../include/CountMin.hpp"




void test_CountMin() {
  CountMin<char> cm(100, 10);
  char data[] = "11111111112345111111";
  for (int i = 0; i < 20; ++i) 
    cm.processItem(data[i]);
  
  for (char i = '0'; i < '6' ; ++i)
    std::cout << i << ": " << cm.estTotWeight(i) << std::endl;
}

int main() {
  test_CountMin();
  return 0;
}
















