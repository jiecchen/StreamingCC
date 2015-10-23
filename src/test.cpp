#include <vector>
#include <iostream>
#include "../include/CountMin.h"


void test_CountMin() {
  CountMin cm(100, 10);
  int data[] = {1,1,1,0,0,0,1,1,2,1,2,3,4,5,5,5,5,5,5,5,5,5,5};
  for (int i = 0; i < 15; ++i) 
    cm.processItem(data[i]);
  
  for (int i = 0; i < 6; ++i)
    std::cout << i << ": " << cm.estTotWeight(i) << std::endl;
}

int main() {
  test_CountMin();
  return 0;
}
















