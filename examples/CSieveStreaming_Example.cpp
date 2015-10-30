#define ARMA
#include "../include/streamcc"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <armadillo>


// convert arma::vec to std::string
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



int main() {

  //========== create data stream =============
  // create a data stream with 10 clusters
  // each cluster has 100 members
  // 
  // in practice, data stream can be read directly
  // from disk or some devices
  int N = 100;  
  std::vector<double> centers;
  for (int i = 0; i < 5000; i += 500) {
    centers.push_back(i);
  }

  Scc::Vectors stream;
  utils::Gaussian gauss(0, 1);
  for (auto i : centers)
    for (int j = 0; j < N; ++j) {
      double ns = gauss.rand();
      arma::vec c({i + ns});
      stream.push_back(c);
    }
  //=========== end ============================



  
  // create an instance using Informative Vector Machine
  Scc::CSieveStreaming<Scc::SieveKnowOptIVM> sv(10, 0.01, 0);
  for (auto &item: stream) {
    sv.update(item);
  }

  // output the cluster centers
  Scc::Vectors &&ans = sv.getCenters(); 
  printVectors(ans);
  
  return 0;
}










