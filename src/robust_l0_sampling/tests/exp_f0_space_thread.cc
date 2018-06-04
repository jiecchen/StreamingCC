// threaded version of exp_f0
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
std::mutex mymutex;

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


// global variables
double actual_t = 0.;
double actual_eps = 0;
double actual_space = 0.;


void run_counter(const std::vector<Point>& dataset, int dimension, double alpha, double eps) {
  int n_groups = count_groups(dataset);
  RobustCounter counter(dimension, alpha, eps);
  Timer timer;
  double max_space = 0.;;
  for (const auto& p : dataset) {
    counter.process(p);
    max_space = std::max(max_space, counter.est_space() + .0);
  }
  std::lock_guard<std::mutex> lock(mymutex);
  actual_t = actual_t + (timer.get_time() + .0) / dataset.size();
  actual_eps = actual_eps + std::abs(n_groups - counter.est_f0() + .0) / n_groups;
  actual_space += max_space;
  
}


void run_f0(const std::vector<Point>& dataset, int dimension, double alpha, double eps) {
  int n_groups = count_groups(dataset);
  RobustF0 counter(dimension, alpha, eps);
  Timer timer;
  double max_space = 0.;;
  for (const auto& p : dataset) {
    counter.process(p);
    max_space = std::max(max_space, counter.est_space() + .0);
  }
  std::lock_guard<std::mutex> lock(mymutex);
  actual_t = actual_t + (timer.get_time() + .0) / dataset.size();
  actual_eps = actual_eps + std::abs(n_groups - counter.est_f0() + .0) / n_groups;
  actual_space += max_space;
}



int main() {
  int dimension = 20;
  auto&& dataset = read_data("dataset/tmp.dat");
  std::cerr << "dim = " << dataset.front().size() << std::endl;
  std::random_shuffle(dataset.begin(), dataset.end());
  std::vector<double> epss = {.3, .1, .05, .03, .02};
  std::vector<double> times;
  std::vector<double> accus;
  std::vector<double> spaces;

  // do experiment for run_counter
  for (auto eps : epss) {
    // reset global variables
    actual_t = 0.;
    actual_eps = 0;
    actual_space = 0.;

    std::vector<std::thread> threads;
    for (int i = 0; i < N; ++i) {
      threads.push_back(std::thread(run_counter, dataset, dimension, alpha, eps));
    }
    for (auto& t : threads) {
      t.join();
    }
    
    accus.push_back(actual_eps / N);
    times.push_back(actual_t / N);
    spaces.push_back(actual_space / N);
  }
  std::cout << "y1 = [" << to_str(spaces, ", ") << "]" << std::endl;
  std::cout << "x1 = [" << to_str(accus, ", ") << "]" << std::endl;



  // do experiment for run_counter
  times.clear();
  accus.clear();
  spaces.clear();
  for (auto eps : epss) {
    // reset global variables
    actual_t = 0.;
    actual_eps = 0;
    actual_space = 0.;

    std::vector<std::thread> threads;
    for (int i = 0; i < N; ++i) {
      threads.push_back(std::thread(run_f0, dataset, dimension, alpha, eps));
    }
    for (auto& t : threads) {
      t.join();
    }
    
    accus.push_back(actual_eps / N);
    times.push_back(actual_t / N);
    spaces.push_back(actual_space / N);
  }
  std::cout << "y2 = [" << to_str(spaces, ", ") << "]" << std::endl;
  std::cout << "x2 = [" << to_str(accus, ", ") << "]" << std::endl;

  
  return 0;
}




