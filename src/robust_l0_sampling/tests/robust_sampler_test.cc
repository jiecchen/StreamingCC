#include "../src/robust_sampler.h"
#include "../src/point.h"
#include "../src/utils.h"

#define BOOST_TEST_MODULE ClassTest
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <random>

std::vector<Point> make_data() {
  const int n_centers = 30;
  std::vector<Point> points;
  for (int i = 1; i <= n_centers; i++) {
    Point p{i * i + .1, i + .1, i * i * i + .1};
    points.push_back(p);
  }
  for (int i = 0; i < n_centers; i++) {
    for (int j = 0; j <= 10; ++j) {
      points.push_back(points[i] + rand_point(3, 0, 0.1));
    }
  }
  return points;
}



BOOST_AUTO_TEST_CASE(RobustSampler2_test) {
  PointHash phash(1);
  double side_len = 3;
  double alpha = 0.9;
  RobustSampler2 sampler(3, alpha);

  auto&& dataset = make_data();
  std::random_shuffle(dataset.begin(), dataset.end());
  // for (auto p : dataset) {
  //   std::cerr << p[0] << " " << to_cell(p, side_len)[0] << std::endl;    
  // }

  std::map<int, int> ct;
  for (int i = 0; i < 10000; i++) {
    RobustSampler sampler(side_len, alpha);
    for (const auto& p : dataset) {
      sampler.process(p);
    }
    auto p = sampler.get_sample();
    auto h = phash(to_cell(p, side_len));
    ct[h] = ct[h] + 1;
  }
  std::cerr << "|ct| = " << ct.size() << std::endl;
  for (auto& p : ct) {
    std::cout << p.first << " " << p.second << std::endl;
  }
}

// BOOST_AUTO_TEST_CASE(RobustSampler_Test) {
//   PointHash phash(1);
//   double side_len = 3;
//   double alpha = 0.9;

//   auto&& dataset = make_data();
//   std::random_shuffle(dataset.begin(), dataset.end());
//   // for (auto p : dataset) {
//   //   std::cerr << p[0] << " " << to_cell(p, side_len)[0] << std::endl;    
//   // }

//   std::map<int, int> ct;
//   for (int i = 0; i < 500; i++) {
//     RobustSampler sampler(side_len, alpha);
//     for (const auto& p : dataset) {
//       sampler.process(p);
//     }
//     auto p = sampler.get_sample();
//     auto h = phash(to_cell(p, side_len));
//     ct[h] = ct[h] + 1;
//   }
//   std::cerr << "|ct| = " << ct.size() << std::endl;
//   for (auto& p : ct) {
//     std::cout << p.first << " " << p.second << std::endl;
//   }
// }


// BOOST_AUTO_TEST_CASE(RobustCounter_Test) {
//   double side_len = 3;
//   double alpha = 0.9;
//   RobustCounter rcounter(3, alpha, 0.3);
  
//   auto&& dataset = make_data();
//   std::random_shuffle(dataset.begin(), dataset.end());

//   for (const auto& p : dataset) {
//     rcounter.process(p);
//   }
//   std::cout << rcounter.to_string()
//             << "\nf0 = " << rcounter.est_f0() << std::endl;     
// }
