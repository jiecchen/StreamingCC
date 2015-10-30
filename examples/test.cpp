#include <vector>
#include <iostream>
#include "../include/streamcc"
#include <random>
#include <map>

using namespace Scc;


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
  CountMin<int> cs(10, 5);
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
  ReservoirSampling<int> usp(10, true);
  for (int i = 0; i < 100; i++)
    usp.processItem(i);
  print_vector(usp.getSamples());
}



// void test_Algebra() {
//   Algebra::Matrix m = Algebra::makeMatrix(2, 2);
//   m[0][0] = m[1][1] = 9;
//   m[0][1] = m[1][0] = 9;
//   double det;
//   Algebra::gaussianElimination(m, det);
//   std::cout << "det(m) = " << det << std::endl;
// }


int main() {
  // test_CountMin();
  // std::cout << "----------------------------------" << std::endl;
  // test_CountSketch();
  test_Sampling();
  // test_Algebra();
  return 0;
}
















