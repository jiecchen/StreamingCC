#include "../src/robust_sampler.h"
#include "../src/point.h"
#include "../src/hash.h"
#include "../src/exp_utils.h"

#include <functional>
#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include <random>
#include <algorithm>



std::map<int, int> ct;
std::mutex my_mutex;
const double alpha = 2.;
const int dimension = 9;
RobustSampler sampler(dimension, alpha);

int cell_hash(const Point& p) {
  return sampler.cell_hash(p);
}


// std::vector<Point> read_data() {
//   int n_centers = 100;
//   std::vector<Point> points;
//   std::vector<Point> centers;
//   for (int i = 1; i <= n_centers; i++) {
//     Point p{i * i + .1};
//     centers.push_back(p);
//   }
//   for (int i = 0; i < n_centers; i++) {
//     for (int j = 0; j <= i; ++j) {
//       points.push_back(centers[i] + rand_point(1, 0, 0.01));
//     }
//   }
//   return points;
// }


void show_stats() {
  std::cout << "|ct| = " << ct.size() << std::endl;
  std::vector<int> frequencies;
  for (const auto& pr : ct) {
    frequencies.push_back(pr.second);
  }
  std::sort(frequencies.begin(), frequencies.end());
  for (const auto& f : frequencies) {
    std::cout << f << std::endl;
  }
}

void run_exp(const std::vector<Point>& dataset, int dimension, double alpha, int n_chunk) {
  for (int i = 0; i < n_chunk; ++i) {
    RobustSampler sampler(dimension, alpha); 
    for (const auto& p : dataset) {
      sampler.process(p);
    }
    Point p;
    try {
      p = sampler.get_sample();
    } catch (const char* e) {
      std::cerr << e << std::endl;
      continue;
    }
    int h = cell_hash(p);
    std::lock_guard<std::mutex> guard(my_mutex);
    ct[h] = ct[h] + 1;
  }
}

int main() {
  auto&& dataset = read_data("dataset/pima-indians-diabetes/small");
  std::cerr << "dim = " << dataset.front().size() << std::endl;
  std::random_shuffle(dataset.begin(), dataset.end());
  std::vector<std::thread> threads;
  int n_repeats = 50000;
  int n_chunk = 5000;
  for (int i = 0; i < n_repeats / n_chunk; ++i) {
    threads.push_back(std::thread(run_exp, dataset, dimension, alpha, n_chunk));
  }
  for (auto& t : threads) {
    t.join();
  }
  show_stats();
  return 0;
}
