#include "../src/sliding_window.h"
#include "../src/robust_sampler.h"
#include "../src/point.h"
#include "../src/hash.h"
#include "../src/exp_utils.h"
#include "../src/timer.h"

#include <functional>
#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include <random>
#include <algorithm>
#include <cassert>



std::map<int, int> ct;
std::mutex my_mutex;
double alpha = 0.1;
int dimension = 5;
const int W = 200;
RobustSampler sampler(dimension, alpha);

int cell_hash(const Point& p) {
  return sampler.cell_hash(p);
}


void run_exp_sw_space(const std::vector<Point>& dataset, int dimension, double alpha) {
  RobustSamplerSW sampler(dimension, alpha, W);
  int max_space = 0;
  for (const auto& p : dataset) {
    sampler.process(p);
    max_space = std::max(max_space, sampler.est_space());
  }
  std::cerr << "sw::space = " << max_space << std::endl;
}


void run_exp_sw_time(const std::vector<Point>& dataset, int dimension, double alpha) {
  RobustSamplerSW sampler(dimension, alpha, W);
  Timer timer;
  timer.reset();
  for (const auto& p : dataset) {
    sampler.process(p);
  }
  std::cerr << "sw::time = " << timer.get_time() / dataset.size() << std::endl;
}



void run_exp_inf_space(const std::vector<Point>& dataset, int dimension, double alpha) {
  RobustSampler sampler(dimension, alpha);
  int max_space = 0;
  for (const auto& p : dataset) {
    sampler.process(p);
    max_space = std::max(max_space, sampler.est_space());
  }
  std::cerr << "inf::space = " << max_space << std::endl;
}


void run_exp_inf_time(const std::vector<Point>& dataset, int dimension, double alpha) {
  RobustSampler sampler(dimension, alpha);
  Timer timer;
  timer.reset();
  for (const auto& p : dataset) {
    sampler.process(p);
  }
  std::cerr << "inf::time = " << timer.get_time() / dataset.size() << std::endl;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: this_binary input_file_with_alpha" << std::endl;
    return 0;
  }
  auto&& dataset_alpha = read_data_with_alpha(argv[1]);
  auto dataset = dataset_alpha.first;
  std::cerr << "|dataset| = " << dataset.size() << std::endl;
  double alpha = dataset_alpha.second;

  dimension = dataset.front().size();
  sampler = RobustSampler(dimension, alpha);
  std::cerr << "dim = " << dimension << ", alpha = " << alpha << std::endl;
  std::random_shuffle(dataset.begin(), dataset.end());
  run_exp_inf_time(dataset, dimension, alpha);
  run_exp_inf_space(dataset, dimension, alpha);
  run_exp_sw_time(dataset, dimension, alpha);
  run_exp_sw_space(dataset, dimension, alpha);

  return 0;
}
