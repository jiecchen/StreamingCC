#define ARMA
#include "../include/streamcc"
#include <iostream>
#include<vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <armadillo>


std::string vec2string(const arma::vec & v) {
  std::stringstream ss;
  ss << "(" << v[0];
  for (int i = 1; i < v.size(); ++i)
    ss << ", " << v[i];
  ss << ")" << std::endl;
  return ss.str();
}


void printVectors(Scc::Vectors &vecs) {
  for (auto &it : vecs) 
    std::cout << vec2string(it) << " ";
  std::cout << std::endl;
}

// random generator function:
int myrandom (int i) { return std::rand()%i;}



void test_CSieveStreaming() {
  int N = 100;
  
  std::vector<double> centers;
  for (int i = 0; i < 5000; i += 500) {
    centers.push_back(i);
  }

  Scc::Vectors stream;
  Gaussian gauss(0, 1);
  for (auto i : centers)
    for (int j = 0; j < N; ++j) {
      double ns = gauss.rand();
      arma::vec c({centers[i] + ns});
      stream.push_back(c);
    }
    
  Scc::CSieveStreaming<Scc::SieveKnowOptIVM> sv(10, 0.05, 0);
  for (auto &item: stream) {
    sv.update(item);
  }
  Scc::Vectors &&ans = sv.getCenters(); 
  printVectors(ans);
}


int main() {
  std::srand ( unsigned ( std::time(0) ) );
  test_CSieveStreaming();
  return 0;
}










