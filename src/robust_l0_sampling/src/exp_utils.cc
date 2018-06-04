#include "exp_utils.h"

#include <sstream>
#include <fstream>
#include <string>
#include <iostream>

// parse a line as a point
Point parse(const std::string& line) {
  std::istringstream iss(line);
  Point p;
  double x;
  while (iss >> x) {
    p.push_back(x);
  }
  return p;
}

std::vector<Point> read_data(std::string file_name) {
  std::vector<Point> dataset;
  std::ifstream fin(file_name);
  std::string line;
  while (std::getline(fin, line)) {
    dataset.push_back(parse(line));
  }
  return dataset;
}

std::pair<std::vector<Point>, double> read_data_with_alpha(std::string file_name) {
  std::vector<Point> dataset;
  std::ifstream fin(file_name);
  double alpha;
  std::string line;
  std::getline(fin, line);
  alpha = std::atof(line.c_str());
  while (std::getline(fin, line)) {
    dataset.push_back(parse(line));
  }
  return {dataset, alpha};
}








