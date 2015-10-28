#include "../include/streamcc"
#include <iostream>
#include<vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace Scc;

// void test_SieveStreaming() {
//   UtilityFunction<int> f(10);
//   for (int i = 0; i < 100; i++) 
//     f.update(i);
//   std::vector<int> S = {0, 35, 100};
//   f.printSamples();
//   std::cout << f(S) << std::endl;
// }

void test_CVector() {
  Vdouble a({1.});
  Vdouble b = {10.};
  Vdouble c = a - b;
  std::cerr << to_string(a) << std::endl << to_string(b) << std::endl;
  std::cerr << to_string(a - b) << std::endl << dist(a, b) << std::endl;
}


void printVectors(const Vectors& vecs) {
  for (auto it = vecs.begin(); it != vecs.end(); ++it)
    std::cout << to_string(*it) << " ";
  std::cout << std::endl;
}

// random generator function:
int myrandom (int i) { return std::rand()%i;}



void test_CSieveStreaming() {
  int N = 10;
  

  std::vector<double> centers;
  for (int i = 0; i < 50000; i += 100) {
    centers.push_back(i);
  }
  Vectors stream;
  Gaussian gauss(0, 1);
  for (size_t i = 0; i < centers.size(); ++i) {
    for (int j = 0; j < N; ++j) {
      double ns = gauss.rand();
      CVector c({centers[i] + ns});
      stream.push_back(c);
    }
  }

    

  CSieveStreaming<SieveKnowOptIVM> sv(10, 0.05, 0);
  for (auto it = stream.begin(); it != stream.end(); ++it)
    sv.update(*it);   
  printVectors(sv.getOptMedoids());
}


int main() {
  std::srand ( unsigned ( std::time(0) ) );
  //  test_SieveStreaming();
  test_CSieveStreaming();
  return 0;
}










