#ifndef _ALGO_UTILS_H
#define _ALGO_UTILS_H

#define ARMA_DONT_USE_WRAPPER
#include "../arma/include/armadillo"

#include "util.h"

#include <cassert>
#include <iostream>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

class vec_idx {
public:
  vec_idx(const arma::vec& v, int i): idx(i), data(v) {}
  int idx;
  arma::vec data;
};


class ClusteringInput {
 public:
  ClusteringInput() {}
  ClusteringInput(const std::vector<arma::vec>& data, const int n_centers,
                  const int n_outliers) {
    this->data = data;
    this->n_centers = n_centers;
    this->n_outliers = n_outliers;
    this->weights = arma::vec(getNrows(), arma::fill::ones);
  }
  std::vector<arma::vec> data;
  int n_centers;
  int n_outliers;
  arma::vec weights;
  std::vector<int> actual_outliers;
  int getNrows() const { return data.size(); }
  int getNcols() const { return data.at(0).n_elem; }

  // Optional class members.
};

class ClusteringOutput {
 public:
  ClusteringOutput() : num_intermediate_centers(0), n_cols(-1) {}
  std::vector<arma::vec> centers;
  // set this if centers are points in the original dataset
  std::vector<int> centers_indices; 
  arma::vec weights;

  std::vector<int> outliers_indices;

  // TODO: remove this because it == centers.size()?
  int num_intermediate_centers;

  // Optional:

  std::vector<int> labels;  // Don't use this for cost calculation
  // External fields
  std::string name;    // This is set before the algorithm runs
  double elapsedTime;  // This is measured and set after the run is done.

  virtual int getNcenters() const { return centers.size(); }
  virtual int getNoutliers() const { return outliers_indices.size(); }
  int getNcols() const {
    if (n_cols > 0)
      return n_cols;
    else if (getNcenters() > 0)
      return centers.at(0).n_elem;
    else
      assert(false);
    return -1;
  }
  void make_sure_labels_weights_outliers(const ClusteringInput& input);
  void fillExtraInfo(const ClusteringInput& input);
  void compute_weights();

  void setNcols(int cols) { n_cols = cols; }

 private:
  int n_cols;
};

class ClusteringAlgo {
 public:
  ClusteringAlgo() : n_nodes(1),verbose(false) {}
  int n_nodes;  // number of used machines should set to one for centralized
                // ones
  bool verbose;
  std::string desc;  // set by renderer experimenter

  std::string getName()
      const {  // the name of the algorithm. We can potentially just class name
    const ClusteringAlgo* ptr = this;
    std::string nameWithNumber = typeid(*ptr).name();
    // Remove all numbers
    nameWithNumber.erase(
        std::remove_if(nameWithNumber.begin(), nameWithNumber.end(), &isdigit),
        nameWithNumber.end());

    return nameWithNumber;
  }
  virtual int maxMsgSize(const ClusteringInput& input) {
    return (input.n_outliers + input.n_centers) * (input.getNcols() + 1) + 2;
  }
  virtual void perform(const ClusteringInput& input,
                       ClusteringOutput& output) = 0;
  virtual std::string paramStr() { return "e"; }
};

void addAlgo(std::vector<std::unique_ptr<ClusteringAlgo>>& algos,
             ClusteringAlgo* newAlgPtr);
#endif
