#ifndef _EXPERIMENT_UTILS_H
#define _EXPERIMENT_UTILS_H

#include <iostream>

class Experiment {
 public:
  Experiment() : verbose(false), n_repeats(1) {}

  bool verbose;
  int n_clusters;
  int n_outliers;
  int n_repeats;
  std::string address = "../data/";
  std::string dataSetName;
  std::string outputFolder;
  // reclusterFunc=;
};

#endif
