#ifndef _DATA_MANAGER_H
#define _DATA_MANAGER_H

#include "../algorithms/util.h"

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include <iostream>
#include <vector>

class DataSet {
 public:
  void load();
  std::string folderName = "../data/";
  std::string fileExtention = ".csv";
  std::vector<arma::vec> data;
  int n_centers;   // Suggested number
  int n_outliers;  // Suggested number
  std::string name;

  static std::map<std::string, double> kmeansCost;   // optimal kmeans
  static std::map<std::string, double> kmedianCost;  // some kmedian value
  static std::vector<std::string> withHeader;

  static double getOptimal(std::string name, util::CostType type);
};

//
// double DataSet::getOptimal(std::string name,util::CostType type) {
//  if(DataSet::kmeansCost.find(name) == DataSet::kmeansCost.end())
//    return -1;
//  else{
//    if(type==util::CostType::k_mean)
//      return DataSet::kmeansCost[name];
//    else
//      return DataSet::kmedianCost[name];
//  }
//}

#endif
