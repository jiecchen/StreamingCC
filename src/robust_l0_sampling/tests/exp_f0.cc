#include "../src/robust_sampler.h"
#include "../src/robust_f0.h"
#include "../src/point.h"
#include "../src/hash.h"
#include "../src/exp_utils.h"
#include "../src/timer.h"
#include "../src/utils.h"

#include <functional>
#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include <random>
#include <algorithm>
#include <cmath>
#include <utility>


const double alpha = 1.;
const int N = 20;


// count # of group in the last window
int count_groups(const std::vector<Point>& dataset) {
  std::set<Point> groups;
  for (const auto& p :  dataset) {
    bool is_new = true;
    for (const auto& pp : groups) {
      if (dist(pp, p) <= alpha) {
        is_new = false;
        break;
      }
    }
    if (is_new) {
      groups.insert(p);
    }
  }
  return groups.size();
}


std::vector<double> run_counter(const std::vector<Point>& dataset, int dimension, double alpha, double eps) {
  double t = 0.;
  double actual_eps = 0;
  double actual_space = 0.;
  int n_groups = count_groups(dataset);
  for (int i = 0; i < N; ++i) {    
    RobustCounter counter(dimension, alpha, eps);
    Timer timer;
    double max_space = 0.;;
    for (const auto& p : dataset) {
      counter.process(p);
      //      max_space = std::max(max_space, counter.est_space() + .0);
    }
    t = t + (timer.get_time() + .0) / dataset.size();
    actual_eps = actual_eps + std::abs(n_groups - counter.est_f0() + .0) / n_groups;
    //    actual_space += max_space;
  }
  return {actual_eps / N, t / N, actual_space / N};
}


std::vector<double> run_f0(const std::vector<Point>& dataset, int dimension, double alpha, double eps) {
  double t = 0.;
  double actual_eps = 0;
  double actual_space = 0.;
  int n_groups = count_groups(dataset);
  for (int i = 0; i < N; ++i) {    
    RobustF0 counter(dimension, alpha, eps);
    Timer timer;
    double max_space = 0.;;
    for (const auto& p : dataset) {
      counter.process(p);
      //      max_space = std::max(max_space, counter.est_space() + .0);
    }
    t = t + (timer.get_time() + .0) / dataset.size();
    actual_eps = actual_eps + std::abs(n_groups - counter.est_f0() + .0) / n_groups;
    //    actual_space += max_space;
  }
  return {actual_eps / N, t / N, actual_space / N};
}


int main() {
  int dimension = 20;
  auto&& dataset = read_data("dataset/tmp.dat");
  std::cerr << "dim = " << dataset.front().size() << std::endl;
  std::random_shuffle(dataset.begin(), dataset.end());
  std::vector<double> epss = {.7, .5, .3, .1, .05};
  std::vector<double> times;
  std::vector<double> accus;
  for (auto eps : epss) {
    auto v = run_counter(dataset, dimension, alpha, eps);
    accus.push_back(v[0]);
    times.push_back(v[1]);
  }
  std::cout << "y1 = [" << to_str(times, ", ") << "]" << std::endl;
  std::cout << "x1 = [" << to_str(accus, ", ") << "]" << std::endl;

  times.clear();
  accus.clear();
  for (auto eps : epss) {
    auto v = run_f0(dataset, dimension, alpha, eps);
    accus.push_back(v[0]);
    times.push_back(v[1]);
  }
  std::cout << "y2 = [" << to_str(times, ", ") << "]" << std::endl;
  std::cout << "x2 = [" << to_str(accus, ", ") << "]" << std::endl;

  
  // std::vector<std::thread> threads;
  // int n_repeats = 1;
  // for (int i = 0; i < n_repeats; ++i) {
  //   threads.push_back(std::thread(run_exp, dataset, dimension, alpha, eps));
  // }
  // for (auto& t : threads) {
  //   t.join();
  // }
  return 0;
}




