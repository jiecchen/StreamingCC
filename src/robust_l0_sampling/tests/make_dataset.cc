#include "../src/exp_utils.h"
#include "../src/point.h"
#include "../src/utils.h"


#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cmath>


std::random_device rd; 
std::mt19937 gen(rd()); 



double rand_double(double l=0., double r=1.) {
  std::uniform_real_distribution<> dis(l, r);
  return dis(gen);
}

// calculate the minimum distance between centers
double calc_min_dist(const std::vector<Point>& centers) {
  double min_dist = 100000000.;

  for (int i = 0; i < centers.size(); ++i) {
    for (int j = i + 1; j < centers.size(); ++j) {
      auto d = dist(centers[i], centers[j]);
      if (min_dist > d) {
	min_dist = d;
      }
    }
  }
  return min_dist;
}


// make data using power law
void make_dataset_pl(const std::vector<Point>& centers) {
  // the size of each group is randomly sampled from {1, 2, ..., nrange}
  double min_dist = calc_min_dist(centers);
  int dimension = centers.front().size();
  double dd = 4. * std::pow(dimension, 1.5);
  double alpha = min_dist / dd;
  std::cerr << "min_dist = " << min_dist
	    << ", alpha = " << alpha 
	    << ", dimension = " << dimension << std::endl; 
  std::cout << 1. << std::endl;
  double x = 0.;
  int n_pts = centers.size();
  for (const Point& c : centers) {
    x += 1;
    int n = std::ceil(n_pts / x);
    for (int i = 0; i < n; ++i) {
      auto p = c * (1. / alpha) + nrand_point(dimension) * rand_double();
      std::cout << to_str(p) << std::endl; 
    }
  }
}



void make_dataset(const std::vector<Point>& centers, int nrange) {
  // the size of each group is randomly sampled from {1, 2, ..., nrange}
  std::uniform_int_distribution<> dis(1, nrange);
  double min_dist = calc_min_dist(centers);
  int dimension = centers.front().size();
  double dd = 4. * std::pow(dimension, 1.5);
  double alpha = min_dist / dd;
  std::cerr << "min_dist = " << min_dist
	    << ", alpha = " << alpha 
	    << ", dimension = " << dimension << std::endl; 
  std::cout << 1. << std::endl;
  for (const Point& c : centers) {
    int n = dis(gen);
    for (int i = 0; i < n; ++i) {
      auto p = c * (1. / alpha) + nrand_point(dimension) * rand_double();
      std::cout << to_str(p) << std::endl; 
    }
  }
}


int main(int argc, char* argv[]) {
  std::string file_name;
  int nrange;
  if (argc == 3) {
    file_name = std::string(argv[1]);
    nrange = atoi(argv[2]);
  } else {
    std::cerr << "Usage: this_binary input_file_of_center nrange. Set range = 0 if use power law" << std::endl;
    return 0;
  }
  auto&& centers = read_data(file_name);
  if (nrange > 0) {
    make_dataset(centers, nrange);
  } else {
    make_dataset_pl(centers);
  }
  return 0;
}
